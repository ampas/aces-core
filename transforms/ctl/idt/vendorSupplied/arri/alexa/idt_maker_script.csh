#!/bin/csh
# Written by A. Forsythe
# Modified by J. Goldstone 10 Mar 2012

foreach v (2 3)
    set eis = (160 200 250 320 400 500 640 800 1000 1280 1600)
    if ($v == 3) then
	set eis = ($eis 2000 2560 3200)
    endif
    foreach ei ($eis)
	mkdir -p -v v$v/EI$ei
	foreach wb (2000 2100 2200 2400 2600 2900 3200 3500 3900 4300 4700 5100 5600 6500 7500 9000 11000)
		python v${v}_IDT_maker.py $ei $wb raw > v${v}/EI$ei/idt-alexav${v}-raw-EI$ei-CCT$wb.ctl
		if ($v == 3) then
		    python v${v}_IDT_maker.py $ei $wb raw nd1pt3 > v${v}/EI$ei/idt-alexav${v}-raw-EI$ei-CCT$wb-ND1pt3.ctl
		endif
	end
	# LogC IDT for a given EI is the same no matter what the CCT
	# and LogC IDT pays no attention to ND 1.3 because presumably that was dealt with
	# in the conversion from some prior encoding to LogC. (If the conversion was done wrong,
	# it should be re-done correctly; we can't guess here in what way it might have been done wrong.)
	if ($v == 2) then
	    foreach wb (2000 2100 2200 2400 2600 2900 3200 3500 3900 4300 4700 5100 5600 6500 7500 9000 11000)
		python v2_IDT_maker.py $ei $wb logc > v2/EI$ei/idt-alexav2-logC-EI$ei-CCT$wb.ctl
	    end
	else
	    # For V3 logC IDTs, CCT is irrelevant. The 'ignore_this' below is a placeholder
	    python v3_IDT_maker.py $ei ignore_this logc > v3/EI$ei/idt-alexav3-logC-EI$ei.ctl
	endif
    end
end
