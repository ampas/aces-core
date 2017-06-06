set -x


# make sure current aces
#pushd $EDRDATA/aces-dev
#git pull https://github.com/ampas/aces-dev.git master
#git commit -m `echo date`
#git push -u origin master
#popd

CTL_MODULE_PATH="$EDRDATA/aces-dev/transforms/ctl/utilities:$EDRDATA/aces-dev/transforms/ctl/lib"

ctlrender -force -verbose \
   -ctl $EDRDATA/aces-dev/transforms/ctl/csc/ADX/ACEScsc.ADX10_to_ACES.ctl \
        -param1 aIn 1.0 \
   -ctl $EDRDATA/aces-dev/transforms/ctl/rrt/RRT.ctl \
   -ctl ./ODT.Academy.Rec2020_P3_ST2084_1000nits.ctl \
  Digital_LAD_2048x1556.dpx -format tiff16 ODT.Academy.Rec2020_P3_ST2084_1000nits.tiff &


ctlrender -force -verbose \
   -ctl $EDRDATA/aces-dev/transforms/ctl/csc/ADX/ACEScsc.ADX10_to_ACES.ctl \
        -param1 aIn 1.0 \
   -ctl $EDRDATA/aces-dev/transforms/ctl/rrt/RRT.ctl \
   -ctl ./ODT.Academy.Rec2020_ST2084_1000nits.ctl \
  Digital_LAD_2048x1556.dpx -format tiff16 ODT.Academy.Rec2020_ST2084_1000nits.tiff &


# make sure all jobs finished
for job in `jobs -p`
do
echo $job
wait $job 
done

exit

