#!/bin/csh
# Written by A. Forsythe

# Build Alexa firmware version directories
echo "Setting up transform directories ..."
mkdir -p v2
mkdir -p v3

# Populate v3 directories with transforms
echo "Generating v3 IDT transforms ..."
foreach encoding (logC raw)
	mkdir -p v3/$encoding
	foreach ei (160 200 250 320 400 500 640 800 1000 1250 1280 1600 2000 2500 3200)
		mkdir -p -v v3/$encoding/EI$ei
		foreach wb (2000 2100 2200 2400 2600 2900 3200 3500 3900 4300 4700 5100 5600 6500 7500 9000 11000)
			python v3_IDT_maker.py $ei $wb $encoding > v3/$encoding/EI$ei/idt-alexav3-$encoding-CCT$wb-EI$ei.ctl
			python v3_IDT_maker.py $ei $wb $encoding nd-1pt3 > v3/$encoding/EI$ei/idt-alexav3-nd1pt3-$encoding-CCT$wb-EI$ei.ctl
		end
	end
end

# Populate v2 directories with transforms
echo "Generating v2 IDT transforms ..."
foreach encoding (logC raw)
	mkdir -p v2/$encoding
	foreach ei (160 200 250 320 400 500 640 800 1000 1250 1600)
		mkdir -p -v v2/$encoding/EI$ei
		foreach wb (2000 2100 2200 2400 2600 2900 3200 3500 3900 4300 4700 5100 5600 6500 7500 9000 11000)
			python v2_IDT_maker.py $ei $wb $encoding > v2/$encoding/EI$ei/idt-alexav2-$encoding-CCT$wb-EI$ei.ctl
		end
	end
end


