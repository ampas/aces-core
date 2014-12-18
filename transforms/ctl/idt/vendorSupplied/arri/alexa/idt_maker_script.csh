#!/bin/csh
# Written by A. Forsythe
# Modified by J. Goldstone 10 Mar 2012 and 20 Nov 2013

set root=.

set wbs = (2000 2100 2200 2400 2600 2900 3200 3500 3900 4300 4700 5100 5600 6500 7500 9000 11000)
foreach sup (2 3)
    set eis = (160 200 250 320 400 500 640 800 1000 1280 1600)
    if (${sup} == 3) then
	set eis = (${eis} 2000 2560 3200)
    endif
    foreach ei (${eis})
	mkdir -p -v ${root}/SUP${sup}/EI${ei}
	foreach wb (${wbs})
		python SUP${sup}_IDT_maker.py ${ei} ${wb} raw > ${root}/SUP${sup}/EI${ei}/IDT.ARRI.Alexa-SUP${sup}-raw-EI${ei}-CCT${wb}.a1.v1.ctl
		if (${sup} == 3) then
		    python SUP${sup}_IDT_maker.py ${ei} ${wb} raw nd1pt3 > ${root}/SUP${sup}/EI${ei}/IDT.ARRI.Alexa-SUP${sup}-raw-EI${ei}-CCT${wb}-ND1pt3.a1.v1.ctl
		endif
	end
	if ($sup == 2) then
	    # SUP2 LogC was still CCT-dependent
	    foreach wb (${wbs})
		python SUP${sup}_IDT_maker.py ${ei} ${wb} logc > ${root}/SUP${sup}/EI${ei}/IDT.ARRI.Alexa-SUP${sup}-logC-EI${ei}-CCT${wb}.a1.v1.ctl
	    end
	else
	    # SUP3 LogC IDT for a given EI is the same no matter what the CCT
	    # and LogC IDT pays no attention to ND 1.3 because presumably that was dealt with
	    python SUP${sup}_IDT_maker.py ${ei} ignore_this logc > ${root}/SUP${sup}/EI${ei}/IDT.ARRI.Alexa-SUP${sup}-logC-EI${ei}.a1.v1.ctl
	endif
    end
end
