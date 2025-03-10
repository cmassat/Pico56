; 6502
;
; Memory subroutines and macros
;
; Copyright (c) 2020 Troy Schrapel
;
; This code is licensed under the MIT license
;
; https://github.com/visrealm/hbc-56
;
;

; -----------------------------------------------------------------------------
; store16: Write a 16-bit immediate value to memory
; -----------------------------------------------------------------------------
; Inputs:
;	.addr: destination address
;	.val16: 16-bit value
; -----------------------------------------------------------------------------
!macro store16 .addr, .val16 {
        lda #<.val16
        sta .addr
        lda #>.val16
        sta .addr + 1
}

; -----------------------------------------------------------------------------
; memcpy: Copy a fixed number of bytes from .src to dest
; -----------------------------------------------------------------------------
; Inputs:
;	.src: source address
;	.dst: destination address
;	.cnt: number of bytes
; -----------------------------------------------------------------------------
!macro memcpy .dst, .src, .cnt {
!if .cnt <= 0 {
	!error "memcpy: invalid count: ", .cnt
} else if .cnt <= 8 {
	!for i, 0, .cnt - 1 {
		lda .src + i
		sta .dst + i
	}
} else { !if .cnt <= 256 {
	ldx #0
-
	lda .src, x
	sta .dst, x
	inx
	cpx #(.cnt & $ff)	
	bne -
} else {
        +store16 MEM_SRC, .src
        +store16 MEM_DST, .dst
        +store16 MEM_LEN, .cnt

        jsr memcpyMultiPage
}
}
}

!macro setMemCpySrc .src {
        +store16 MEM_SRC, .src
}

!macro setMemCpyDst .dst {
        +store16 MEM_DST, .dst
}

!macro setMemCpySrcInd .srcRef {
	lda .srcRef
	sta MEM_SRC
	lda .srcRef + 1
	sta MEM_SRC + 1
}

!macro setMemCpyDstInd .dstRef {
	lda .dstRef
	sta MEM_DST
	lda .dstRef + 1
	sta MEM_DST + 1
}

!macro memcpySinglePage .bytes {

	!if .bytes <= 0 {
		!error "memcpySinglePage: invalid count: ", .bytes
	}	

	ldy #.bytes
	jsr memcpySinglePage
}


; -----------------------------------------------------------------------------
; memcpyPort: Copy a fixed number of bytes from .src ram to dest port
; -----------------------------------------------------------------------------
; Inputs:
;	.src: source address
;	.dst: destination address
;	.cnt: number of bytes
; -----------------------------------------------------------------------------
!macro memcpyPort .dst, .src, .cnt {
	!if .cnt <= 0 {
		!error "memcpyPort: invalid count: ", .cnt
	}
	
        +store16 MEM_SRC, .src
        +store16 MEM_DST, .dst

	!if .cnt <= 256 {
		ldy #<.cnt					
		jsr memcpySinglePagePort
	} else {
                +store16 MEM_LEN, .cnt
		jsr memcpyMultiPagePort
	}
}

!zone {

; -----------------------------------------------------------------------------
; memset: Set a fixed number of bytes to a single value
; -----------------------------------------------------------------------------
; Inputs:
;	.dst: destination address
;	val: the byte value
;	.cnt: number of bytes
; -----------------------------------------------------------------------------
!macro memset .dst, .val, .cnt {
!if .cnt <= 0 {
	!error "memset: invalid count: ", .cnt
} else if .cnt <= 8 {
	lda #.val
	!for i, 0, .cnt - 1 {
	sta .dst + i
	}
} else if .cnt <= 256 {
	ldx #<.cnt
	lda #.val
-
	dex
	sta .dst, x
	cpx #0
	bne -
} else {
        +store16 MEM_DST, .dst
        +store16 MEM_LEN, .cnt
        
	lda #.val
	jsr memsetMultiPage
}
}


}