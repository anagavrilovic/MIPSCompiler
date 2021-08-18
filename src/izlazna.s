.globl main

.data
m1:		.word 6
m2:		.word 5

.text
main:
	la		$t0, m1
	lw		$t2, 0($t0)
	la		$t0, m2
	lw		$t1, 0($t0)
	add		$t0, $t2, $t1
	and		$t2, $t0, $t1
	sll		$t2, $t2, 10
	srl		$t2, $t2, 10
	bne		$t2, $t2, main
