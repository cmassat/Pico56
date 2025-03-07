!src "hbc56kernel.inc"

!src "zeropage.asm"


hbc56Meta:
        +setHbcMetaTitle "HBC-56 TestApp"
        +setHbcMetaNES
        rts

; -----------------------------------------------------------------------------
; main entry point
; -----------------------------------------------------------------------------
hbc56Main:

    rts