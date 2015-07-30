#!/usr/bin/php
<?php

# Copyright (c) 2015 Sam Trenholme
#
# TERMS
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# This software is provided 'as is' with no guarantees of correctness or
# fitness for purpose.

class rg32 {
	protected $belt;
	protected $mill;
	protected $phase;
	
	protected function init_rg() {
		# RG32 constants
		$this->mill = array();
		$this->belt = array();
	
		# Init mill	
		for($a = 0; $a < 19; $a++) {
			$this->mill[$a] = 0;
		}
	
		# Init belt
		for($a = 0; $a < 3; $a++) {
			$this->belt[$a] = array();
			for($b = 0; $b < 13; $b++) {
				$this->belt[$a][$b] = 0;
			}
		}
	
		# Which word to show on output
		$this->phase = 2;
	}

	protected function millfunction() {
		$A = array(); # Copy of mill
		$rotate = 0;
		$tmp = 0;

		for($i=0;$i<19;$i++) {
			# This is a more concise way of expressing
			# triangular numbers than in the spec
			# r=c+r&31 in C (& lower precedence than +)
			$rotate = ($rotate + $i) & 31;

			$g = $i * 7;
			$tmp = $this->mill[$g % 19];
			$tmp ^= $this->mill[($g + 1) % 19] |
				($this->mill[($g + 2) % 19] ^
				 0xffffffff);
			# PHP32 can not do right shifts on 32-bit words
			# without an ugly hack, since PHP does not have 
			# a logical right shift.  
			if(!$rotate) {$A[$i] = $tmp;} else {
				$A[$i] = ((($tmp >> $rotate) & 
					(0x7fffffff >> ($rotate - 1)))
				| ($tmp << (32 - $rotate))) &
				  0xffffffff;
			}
		}

		for($i=0;$i<19;$i++) {
			$this->mill[$i] = $A[$i] ^
				$A[($i + 1) % 19] ^
				$A[($i + 4) % 19];
		}
		
		$this->mill[0] ^= 1;

	}
			
	protected function beltfunction() {
		$beltsave = array();

		# Belt function: Simple rotation (Page 9)
		for($beltrow = 0; $beltrow < 3; $beltrow++) {
			$beltsave[$beltrow] = $this->belt[$beltrow][12];
			for($beltcol = 12; $beltcol > 0; $beltcol--) {
				$this->belt[$beltrow][$beltcol] = 
					$this->belt[$beltrow][$beltcol - 1];	
			}
			$this->belt[$beltrow][$beltcol] = $beltsave[$beltrow];
		}

		# Mill->Belt feedforward (Page 9)
		for($i = 0; $i < 12; $i++) {
			$this->belt[$i % 3][$i + 1] ^= $this->mill[$i + 1];
		}

		# Mill 
		$this->millfunction();

		# Belt->Mill feedforward
		for($i = 0; $i < 3; $i++) {
			$this->mill[13 + $i] ^= $beltsave[$i];
		}

	}

	protected function input_map($in) {
		for(;;) {
			for($a = 0; $a < 3; $a++) {
				$s = 0;
				for($q = 0; $q < 4; $q++) {
					$w = ord($in);
					if(strlen($in) < 1) {
						$w = 1;
					}
					$s |= $w << (8 * $q);
					if(strlen($in) < 1) {	
						$this->belt[$a][0] ^= $s;
						$this->mill[$a + 16] ^= $s;
						for($c = 0; $c < 17; $c++) {
							$this->beltfunction();
						}
						return;
					}
					$in = substr($in, 1);
				}
				$this->belt[$a][0] ^= $s;
				$this->mill[$a + 16] ^= $s;
			}
			$this->beltfunction();
		}	
	}

	public function rg() {
		if($this->phase == 2) {
			$this->phase = 1;
			$this->beltfunction();
		} else {
			$this->phase = 2;
		}
		$i = $this->mill[$this->phase];
		$i = (($i & 0xff) << 24) |
		     (($i & 0xff00) << 8) |
		     (($i & 0xff0000) >> 8) |
		     # Again, PHP32 has right-shift issues
		     (($i >> 24) & 0xff);
		return $i;
	}

	public function __construct($seed) {
		$this->init_rg();
		$this->input_map($seed);
	}

}

# This is testing code which I used to make sure this code
# generates correct test vectors.
#@$in = $argv[1];
#$test = new rg32($in);
#for($a=0;$a<8;$a++) {
#	printf("%08x", $test->rg());
#}
#print "\n";

?>
