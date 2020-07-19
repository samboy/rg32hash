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
	
	# RG32 parameters
	protected $millsize;
	protected $beltwidth;
	protected $beltheight;
	protected $wordsize; 	
	protected $mask;
	protected $rmask;
	protected $initval;
	protected $mill2belt;
	protected $m2bOffsetB;
	protected $m2bOffsetM;
	protected $b2mOffset;
	protected $gammaOffset;
	protected $millrounds;
	protected $theta;
	protected $thetaXOR;
	protected $iota;
	protected $iotaXOR;
	
	# We make the core flexible to allow a lot of
	# rg variants to play with
	protected function rg32_params() {
		$this->millsize = 19;
		$this->beltwidth = 13;
		$this->beltheight = 3;
		$this->mill2belt = $this->beltwidth - 1;
		$this->m2bOffsetB = 1;
		$this->m2bOffsetM = 1;
		$this->b2mOffset = $this->beltwidth;	
		$this->gammaOffset = 7;
		$this->theta = array(0,1,4);
		$this->thetaXOR = 0;
		$this->iota = array(0);
		$this->iotaXOR = 1;
		$this->wordsize = 32;
		$this->mask = 0xffffffff;
		$this->rmask = 0x7fffffff;
		$this->initval = 0;
		$this->millrounds = 1;
	}

	protected function init_rg() {
		# RG32 constants
		$this->mill = array();
		$this->belt = array();
	
		# Init mill	
		for($a = 0; $a < $this->millsize; $a++) {
			$this->mill[$a] = $this->initval;
		}
	
		# Init belt
		for($a = 0; $a < $this->beltheight; $a++) {
			$this->belt[$a] = array();
			for($b = 0; $b < $this->beltwidth; $b++) {
				$this->belt[$a][$b] = $this->initval;
			}
		}
	
		# Which word to show on output
		$this->phase = 2;
	}

	protected function millfunction() {
		$A = array(); # Copy of mill
		$rotate = 0;
		$tmp = 0;

		for($i=0;$i<$this->millsize;$i++) {
			# This is a more concise way of expressing
			# triangular numbers than in the spec
			# r=c+r&31 in C (& lower precedence than +)
			$rotate = ($rotate + $i) % $this->wordsize;

			$g = $i * $this->gammaOffset;
			$tmp = $this->mill[$g % $this->millsize];
			$tmp ^= $this->mill[($g + 1) % $this->millsize] |
				($this->mill[($g + 2) % $this->millsize] ^
				 $this->mask);
			# PHP32 can not do right shifts on 32-bit words
			# without an ugly hack, since PHP does not have 
			# a logical right shift.  Hence, rmask.
			if(!$rotate) {$A[$i] = $tmp;} else {
				$A[$i] = ((($tmp >> $rotate) & 
					($this->rmask >> ($rotate - 1)))
				| ($tmp << ($this->wordsize - $rotate))) &
				  $this->mask;
			}
		}

		for($i=0;$i<$this->millsize;$i++) {
			$this->mill[$i] = $this->thetaXOR;
			foreach($this->theta as $t) {
				$this->mill[$i] ^= $A[($i + $t) % 
					$this->millsize];
			}
		}
		
		foreach($this->iota as $io) {
			$this->mill[$io] ^= $this->iotaXOR;
		}

	}
			
	protected function beltfunction() {
		$beltsave = array();
		$millsave = array();

		# Belt function: Simple rotation (Page 9)
		for($beltrow = 0; $beltrow < $this->beltheight; $beltrow++) {
			$beltsave[$beltrow] = $this->belt[$beltrow][
					$this->beltwidth - 1];
			for($beltcol = $this->beltwidth - 1; 
					$beltcol > 0; $beltcol--) {
				$this->belt[$beltrow][$beltcol] = 
					$this->belt[$beltrow][$beltcol - 1];	
			}
			$this->belt[$beltrow][$beltcol] = $beltsave[$beltrow];
		}

		# Mill->Belt feedforward (Page 9)
		for($i = 0; $i < $this->mill2belt; $i++) {
			$this->belt[$i % $this->beltheight][
				($i + $this->m2bOffsetB) % $this->beltwidth] ^=
				$this->mill[($i + $this->m2bOffsetM) % 
				$this->millsize];
		}

		# Mill 
		for($i = 0; $i < $this->millrounds; $i++) {
			$this->millfunction();
		}

		# Belt->Mill feedforward
		for($i = 0; $i < $this->beltheight; $i++) {
			$this->mill[$this->b2mOffset + $i] ^= $beltsave[$i];
		}

	}

	# This code assumes 3 words to map and 32-bit words
	# It also assumes that input is always a multiple of 
	# eight bits long (RadioGatún is not defined for non-octet
	# input), and hard-codes 0x01 as RadioGatún’s padding (the
	# padding is not defined when RadioGatún’s word length is not
	# a multiple of 8)
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

	# Create a 32-bit word based on RG’s internal state
	# This code assumes that we pull two words from the mill before
	# running the beltmill, that 32-bit words are little-endian, that
	# the words are 32 bits long
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

	protected function debug_show_mill() {
		for($a=0;$a<$this->millsize;$a++) {
			printf("m[%2d] = 0x%08x ",$a,$this->mill[$a]);
			if($a % 4 == 3) {print "\n";}
		}
		print "\n\n";
	}

	protected function debug_show_belt() {
		for($a=0;$a<$this->beltheight;$a++) {
			for($b=0;$b<$this->beltwidth;$b++) {
				printf("b[%d][%2d] = 0x%08x ",
					$a, $b, $this->belt[$a][$b]);
				if(($b % 3) == 2) {print "\n";}
			}
			print "\n";
		}
	}

	public function __construct($seed) {
		$this->rg32_params();
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
