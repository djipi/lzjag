;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; JAGUAR Multimedia Entertainment System Source Code
;;;
;;;	COPYRIGHT (c)1994 Atari Computer Corporation
;;;	UNAUTHORIZED REPRODUCTION, ADAPTATION, DISTRIBUTION,
;;;	PERFORMANCE OR DISPLAY OF THIS COMPUTER PROGRAM OR
;;;	THE ASSOCIATED AUDIOVISUAL WORK IS STRICTLY PROHIBITED.
;;;	ALL RIGHTS RESERVED.
;;;
;;;	Module: delzjag.s
;;;		GPU Code to Un-LZSS a Block of Memory
;;;
;;;   History: 09/20/94 - Created (SDS)
;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		.include	"jaguar.inc"

;;; Exposed Globals
		.globl		_dlzstart
		.globl		_dlzend
		
		.globl		delzss

		.globl		lzinbuf
		.globl		lzoutbuf
		.globl		lzworkbuf

;;; Equates
INDEX_BIT_COUNT	.equ	13
LENGTH_BIT_COUNT	.equ	4
BREAK_EVEN		.equ	((1+INDEX_BIT_COUNT+LENGTH_BIT_COUNT)/9)		

		.68000
		.phrase
_dlzstart:
		.gpu
		.org	G_RAM+$100

;;; Register Equates
lzinbufptr	.equr	r0
lzoutbufptr	.equr	r1
lzworkbufptr	.equr	r2

currentpos	.equr	r3
ch		.equr	r4

addr		.equr	r5
temp		.equr	r6

matchlen	.equr	r7
matchpos	.equr	r8
mask		.equr	r9
rack		.equr	r10
bufmask	.equr	r11
bigmask	.equr	r12
startmask	.equr	r13

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Here are some Macros that make things more readable
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; INPUT_BIT (leave zero flag indicating bit state

MACRO	INPUT_BIT
		cmp	startmask,mask
		jr	NE,.a\~
		nop

		loadb	(lzinbufptr),rack	; Get a new byte
		addq	#1,lzinbufptr
.a\~:
		move	rack,temp	; Make a copy
		and	mask,temp	; Isolate bit

		shrq	#1,mask		; mask >>= 1
		jr	NE,.b\~
		nop

		move	startmask,mask	; If mask == 0, mask = $80
.b\~:
		cmpq	#0,temp		; Do final test to set ZERO flag
ENDM

;;; INPUT_BITS destreg,num_bits

MACRO	INPUT_BITS	destreg, num_bits
		moveq	#1,bigmask	; bigmask = 1 << (bit_count - 1)
		shlq	#\num_bits-1,bigmask
		
		moveq	#0,\destreg
.m\~:
		cmpq	#0,bigmask	; If bigmask == 0 we're done
		movei	#.p\~,temp
		jump	EQ,(temp)
		nop
		
		cmp	startmask,mask
		jr	NE,.n\~
		nop

		loadb	(lzinbufptr),rack	; Load new input byte
		addq	#1,lzinbufptr
.n\~:
		move	rack,temp	; if( rack & mask ) destreg |= bigmask
		and	mask,temp

		jr	EQ,.o\~
		nop

		or	bigmask,\destreg
.o\~:
		shrq	#1,bigmask	; bigmask >>= 1
		shrq	#1,mask		; mask >>= 1
		
		movei	#.m\~,temp
		jump	NE,(temp)
		nop

		move	startmask,mask
		jump	T,(temp)
		nop
.p\~:
ENDM

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; This is our routine entry point
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
delzss:
		movei	#lzinbuf,addr
		load	(addr),lzinbufptr

		addq	#4,addr
		load	(addr),lzoutbufptr

		addq	#4,addr
		load	(addr),lzworkbufptr

		moveq	#1,mask
		movei	#$1FFF,bufmask
		shlq	#7,mask
		moveq	#1,currentpos
		move	mask,startmask
bigloop:
		INPUT_BIT

		movei	#cstream,addr
		jump	EQ,(addr)		; If 0, get compressed stream	
		nop

		INPUT_BITS	ch,8		; Else, get byte
		
		storeb	ch,(lzoutbufptr)	; store byte
		addq	#1,lzoutbufptr		; go to next buffer pos

		move	lzworkbufptr,addr  	; get address of window
		add	currentpos,addr		; add offset
		storeb	ch,(addr)		; update window

		addq	#1,currentpos		; increment window pointer
		and	bufmask,currentpos	; force range of 0-8191

		movei	#bigloop,addr		; Loop again
		jump	T,(addr)
		nop
cstream:
		INPUT_BITS	matchpos,13	; Get Index into Window

		cmpq	#0,matchpos		; END_OF_STREAM???

		movei	#alldone,addr
		jump	EQ,(addr)
		nop
		
		INPUT_BITS	matchlen,4	; Get Length of Match
		addq	#BREAK_EVEN,matchlen
getchs:
		move	lzworkbufptr,addr  	; Get Window Address + Offset
		add	matchpos,addr

		loadb	(addr),ch		; Load a byte from window
		or	ch,ch
		storeb	ch,(lzoutbufptr)	; Store it to our buffer
		addq	#1,lzoutbufptr
					   
		move	lzworkbufptr,temp  	; Store byte in window
		add	currentpos,temp		; @ currentpos
		storeb	ch,(temp)
		
		addq	#1,currentpos		; Update Window Position
		and	bufmask,currentpos
		
		addq	#1,matchpos		; Increment window read addr
		and	bufmask,matchpos
		subq	#1,matchlen		; Decrement loop counter

		movei	#getchs,temp		; Get next character
		jump	PL,(temp)
		nop

		movei	#bigloop,addr		; Loop again
		jump	T,(addr)
		nop

;;; Ok, we're done... now leave.
alldone:
		moveq	#0,temp
		movei	#G_CTRL,addr
		store	temp,(addr)
		nop

		.long

lzinbuf: 	.dc.l	0		; Pointer to Compressed Data
lzoutbuf:	.dc.l	0		; Pointer of Destination Buffer
lzworkbuf:	.dc.l	0		; Pointer to 8k LZSS Window
debug:		.dc.l	0

		.68000
_dlzend:
		.end