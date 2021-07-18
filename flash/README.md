# Flash dump from PocketGo S30

There is one SPI flash device of 8MB divided in 6 partitions. It can be made writable for modification.
There does not seem to be any copyrighted material that couldn't be included here.

|Partiton |Purpose     |Size |
|---------|------------|-----|
|mtdblock0|u-boot      |384KB| 
|mtdblock1|boot config |128KB| 
|mtdblock2|boot image  |7296KB| 
|mtdblock3|squashfs (empty) |128KB| 
|mtdblock4|unknown     |256KB| 
|mtdblock5|unknown     |0KB  | 
