
const int gamutCuspTableSize = 360;

// Table generation
const Chromaticities REACH_PRI =    // equal to ACES "AP1" primaries
{
    { 0.713,    0.293},
    { 0.165,    0.830},
    { 0.128,    0.044},
    { 0.32168,  0.33767}
};  


const float AP0_XYZ_TO_RGB[3][3] = XYZtoRGB_f33( AP0, 1.0);
const float AP0_RGB_TO_XYZ[3][3] = RGBtoXYZ_f33( AP0, 1.0);

const float AP1_XYZ_TO_RGB[3][3] = XYZtoRGB_f33( AP1, 1.0);
    


float[3] RGB_to_JMh( float RGB[3], 
                     float RGB_TO_XYZ_M[3][3],
                     float peakLuminance )
{
    float luminanceRGB[3] = mult_f_f3( peakLuminance, RGB);
    float XYZ[3] = mult_f3_f33( luminanceRGB, RGB_TO_XYZ_M );
    
    float RGB_w[3] = {100., 100., 100.};
    float XYZ_w[3] = mult_f3_f33( RGB_w, RGB_TO_XYZ_M );
//     float XYZ_w[3] = {95.0456, 100., 108.906};  // D65
    float JMh[3] = XYZ_to_Hellwig2022_JMh( XYZ, XYZ_w );
//     print( "lumRGB\t", luminanceRGB[0], "\t", luminanceRGB[1], "\t", luminanceRGB[2], "\n");
//     print( "XYZ\t", XYZ[0], "\t", XYZ[1], "\t", XYZ[2], "\n");
//     print( "limitWhite\t", XYZ_w[0], "\t", XYZ_w[1], "\t", XYZ_w[2], "\n");
//     print( "JMh\t", JMh[0], "\t", JMh[1], "\t", JMh[2], "\n\n");

    return JMh;
}

float[3] JMh_to_RGB( float JMh[3], 
                     float XYZ_TO_RGB_M[3][3],
                     float peakLuminance )
{
    float RGB_w[3] = {100., 100., 100.};
    float XYZ_w[3] = mult_f3_f33( RGB_w, invert_f33( XYZ_TO_RGB_M) );
//     float XYZ_w[3] = {95.0456, 100., 108.906};  // D65
    float luminanceXYZ[3] = Hellwig2022_JMh_to_XYZ( JMh, XYZ_w );
    float luminanceRGB[3] = mult_f3_f33( luminanceXYZ, XYZ_TO_RGB_M );
    float RGB[3] = mult_f_f3( 1./ peakLuminance, luminanceRGB);

    return RGB;
}





bool any_below_zero( float newLimitRGB[3]) 
{
    return (newLimitRGB[0] < 0. || newLimitRGB[1] < 0. || newLimitRGB[2] < 0.);
}

float[gamutCuspTableSize][3] make_gamut_table( Chromaticities C,
                                               float peakLuminance ) {

    const float RGB_TO_XYZ_M[3][3] = RGBtoXYZ_f33( C, 1.0);

    float gamutCuspTableUnsorted[gamutCuspTableSize][3];
    
    int i;
    for (i = 0; i < gamutCuspTableSize; i=i+1) {
        float i_float = i;
        float hNorm = i_float / gamutCuspTableUnsorted.size;
    
        float HSV[3] = {hNorm, 1., 1.};
        float RGB[3] = HSV_to_RGB( HSV); 

        gamutCuspTableUnsorted[i] = RGB_to_JMh( RGB, 
                                                RGB_TO_XYZ_M,
                                                peakLuminance );
//         print( "HSV\t", HSV[0], "\t", HSV[1], "\t", HSV[2], "\n");
//         print( "RGB\t", RGB[0], "\t", RGB[1], "\t", RGB[2], "\n");
//         print( "JMh\t", gamutCuspTableUnsorted[i][0], "\t", gamutCuspTableUnsorted[i][1], "\t", gamutCuspTableUnsorted[i][2], "\n");
    }

    int minhIndex = 0;
    for (i = 0; i < gamutCuspTableSize; i=i+1) {
        if ( gamutCuspTableUnsorted[i][2] < gamutCuspTableUnsorted[minhIndex][2])
            minhIndex = i;
    }

    float gamutCuspTable[gamutCuspTableSize][3];
    for (i = 0; i < gamutCuspTableSize; i=i+1) {
        gamutCuspTable[i] = gamutCuspTableUnsorted[(minhIndex+i) % gamutCuspTableSize];
//         print_f3(gamutCuspTable[i]);
    }
    
    return gamutCuspTable;
}


float[gamutCuspTableSize] make_gamut_reach_table( Chromaticities C,
                                                  float limitJmax, 
                                                  float peakLuminance )
{
    const float XYZ_TO_RGB_M[3][3] = XYZtoRGB_f33( C, 1.0);

    float gamutReachTable[gamutCuspTableSize];

//     print( "\ngamutReachTable: \n");

    int i;
    for (i = 0; i < gamutCuspTableSize; i=i+1) {
        float i_float = i;
        float hue = base_hue_for_position( i, gamutCuspTableSize);

        float search_range = 50.;
        float low = 0.;
        float high = low + search_range;
        bool outside = false;

        while ((outside != true) & (high < 1300.))
        {
            float searchJMh[3] = { limitJmax, high, hue };
            float newLimitRGB[3] = JMh_to_RGB( searchJMh, 
                                               XYZ_TO_RGB_M,
                                               peakLuminance );
            outside = any_below_zero( newLimitRGB);
            if (outside == false) {
                low = high;
                high = high + search_range;
            }
        }

        while (high-low > 1e-2) 
        {
            float sampleM = (high + low) / 2.;
            float searchJMh[3] = { limitJmax, sampleM, hue };
            float newLimitRGB[3] = JMh_to_RGB( searchJMh, 
                                               XYZ_TO_RGB_M,
                                               peakLuminance );
            outside = any_below_zero( newLimitRGB);
            if (outside) {
                high = sampleM;
            } else {
                low = sampleM;
            }
        }
        
        gamutReachTable[i] = low;
//         print(gamutReachTable[i],"\n");
    }
    return gamutReachTable;
    
}




bool outside_hull( float newLimitRGB[3] )
{
    // limit value, once we cross this value, we are outside of the top gamut shell
    float maxRGBtestVal = 1.0;

    return newLimitRGB[0] > maxRGBtestVal || newLimitRGB[1] > maxRGBtestVal || newLimitRGB[2] > maxRGBtestVal;
}


bool evaluate_gamma_fit( float JMcusp[2], 
                         float testJMh[][3], 
                         ODTParams PARAMS, 
                         float topGamma,
                         float peakLuminance )
{
    float midJ = PARAMS.midJ;
    float limitJmax = PARAMS.limitJmax;
    float lowerHullGamma = PARAMS.lowerHullGamma;
    float focusDist = PARAMS.focusDist;

    float focusJ = lerp(JMcusp[0], midJ, min(1.0, cuspMidBlend - (JMcusp[0] / limitJmax)));

//     print( "size\n", testJMh.size, "\n");
    for (int testIndex = 0; testIndex < testJMh.size; testIndex = testIndex + 1)
    {
        float slope_gain = limitJmax * focusDist * getFocusGain( testJMh[testIndex][0], 
                                                                 JMcusp[0], 
                                                                 limitJmax );
        float approxLimit[3] = findGamutBoundaryIntersection( testJMh[testIndex], 
                                                              JMcusp, 
                                                              focusJ, 
                                                              limitJmax, 
                                                              slope_gain, 
                                                              topGamma, 
                                                              lowerHullGamma );
        float approximate_JMh[3] = { approxLimit[0], approxLimit[1], testJMh[testIndex][2] };

        float newLimitRGB[3] = JMh_to_RGB( approximate_JMh, 
                                           PARAMS.LIMIT_XYZ_TO_RGB, 
                                           peakLuminance );

        if (!outside_hull(newLimitRGB)) {
           return false;
        }
    }
    return true;
}


float[gamutCuspTableSize] make_upper_hull_gamma( float gamutCuspTable[][3], 
                                                 ODTParams PARAMS,
                                                 float peakLuminance ) {
    // Find upper hull gamma values for the gamut mapper
    // Start by taking a h angle
    // Get the cusp J value for that angle
    // Find a J value halfway to the Jmax
    // Iterate through gamma values until the approximate max M is negative through the actual boundary

    // positions between the cusp and Jmax we will check
    // variables that get set as we iterate through, once all are set to true we break the loop

    const int test_count = 3;
    float testPositions[test_count] = {0.01, 0.5, 0.99};
    float gamutTopGamma[gamutCuspTableSize];

    for (int i = 0; i < gamutCuspTableSize; i = i + 1)
//     for (int i = 0; i < 1; i = i + 1)
    {
//         print( "\n\n\nBEGINNING HUE " , i, "\n");
        // default value. This will get overridden as we loop, but can be a good diagnostic 
        // to make sure things are working
        gamutTopGamma[i] = -1.;
        
        // get cusp from cusp table at hue position
        float hue = base_hue_for_position(i, gamutCuspTableSize);
        float JMcusp[2] = cuspFromTable( hue, gamutCuspTable);
        
        float testJMh[test_count][3];
        // create test values halfway between the cusp and the Jmax
        for (int testIndex = 0; testIndex < testJMh.size; testIndex = testIndex + 1) {
            float testJ = JMcusp[0] + ((PARAMS.limitJmax - JMcusp[0]) * testPositions[testIndex]);
            testJMh[testIndex][0] = testJ;
            testJMh[testIndex][1] = JMcusp[1];
            testJMh[testIndex][2] = hue;
        }

        float search_range = 0.4;
        float low = 0.;
        float high = low + search_range;
        bool outside = false;
        
        while (!(outside) && (high < 5.0)) 
        {
            bool gammaFound = evaluate_gamma_fit( JMcusp, 
                                                  testJMh, 
                                                  PARAMS,
                                                  high, 
                                                  peakLuminance );
            if (!gammaFound) {
                low = high;
                high = high + search_range;
            } else {
                outside = true;
            }

        }
        
        float testGamma = -1.0;
        while ( (high-low) > 1e-5)
        {
            testGamma = (high + low) / 2.;
//             print( "low\t", low, "\thigh\t", high, "\n");
            bool gammaFound = evaluate_gamma_fit( JMcusp, 
                                                  testJMh, 
                                                  PARAMS, 
                                                  testGamma,
                                                  peakLuminance );
            if (gammaFound) {
                high = testGamma;
                gamutTopGamma[i] = high;
            } else {
                low = testGamma;
            }
        }

        if (gamutTopGamma[i] < 0.) {
            print( "Did not find top gamma for hue: ", hue, "\n");
        }
//         print( "hue " , hue, ":\t\t",gamutTopGamma[i], "\n");
//         print( gamutTopGamma[i], "\n");
    }

    return gamutTopGamma;
}


float[3] viewingConditionsToSurround( int viewingConditions = 1) {
    float surround[3];

    // hack to turn incoming int value into surround coeffs
    if (viewingConditions == 0) {                // "Dark"
        surround[0] = 0.8; surround[1] = 0.525; surround[2] = 0.8;
    } else if (viewingConditions == 1) {         // "Dim
        surround[0] = 0.9; surround[1] = 0.59; surround[2] = 0.9;
    } else if (viewingConditions == 2) {         // "Average"
        surround[0] = 1.0; surround[1] = 0.69; surround[2] = 1.0;
    } else {
        
    }
    return surround;
}


struct CAMParams
{
    // CAM
    float referenceLuminance;
    float L_A;                  // luminance of adapting field
    float Y_b;                  // luminance of background

    float ac_resp;              // achromatic response
    float ra;
    float ba;
    
    float surround[3];          
    
    float MATRIX_16[3][3];      // LMS matrix from custom CAM primaries
    float panlrcm[3][3];        // Matrix for Hellwig inverse
    
    float XYZ_w_in[3];
};




ODTParams init_ODTParams( 
    float peakLuminance,
    Chromaticities limitingPrimaries,
    Chromaticities encodingPrimaries,
    float viewingConditions = 1         // 0 = "dark"; 1 = "dim"; 2 = "average"
)
{
//     CAMParams CAMPARAMS = init_CAMParams( viewingConditions);
    TSParams TSPARAMS = init_TSParams( peakLuminance);

    float limitJmax = Y_to_Hellwig_J( peakLuminance );
    float midJ = Y_to_Hellwig_J( TSPARAMS.c_t * 100. );

    const Chromaticities INPUT_PRI =    // equal to ACES "AP0" primaries
        {
          { 0.73470,  0.26530},
          { 0.00000,  1.00000},
          { 0.00010, -0.07700},
          { 0.32168,  0.33767}
        };  
    
    // Chroma compress presets
    const float chroma_compress = 2.4;
    const float chroma_compress_fact = 3.3;
    const float chroma_expand = 1.3;
    const float chroma_expand_fact = 0.69;
    const float chroma_expand_thr = 0.5;

    // Calculated chroma compress variables
    const float log_peak = log10( TSPARAMS.n / TSPARAMS.n_r);
    const float compr = chroma_compress + (chroma_compress * chroma_compress_fact) * log_peak;
    const float sat = max(0.2, chroma_expand - (chroma_expand * chroma_expand_fact) * log_peak);
    const float sat_thr = chroma_expand_thr / TSPARAMS.n;

    const float surround[3] = surround;
    const float model_gamma = 1. / (surround[1] * (1.48 + sqrt( Y_b / L_A )));

    const float focusDist = focusDistance + 
                            focusDistance * focusDistanceScaling * log_peak;

    const float RGB_w[3] = {100., 100., 100.};

    // Input
    const float INPUT_RGB_TO_XYZ[3][3] = RGBtoXYZ_f33( INPUT_PRI, 1.0);
    const float INPUT_XYZ_TO_RGB[3][3] = XYZtoRGB_f33( INPUT_PRI, 1.0);
    float XYZ_w_in[3] = mult_f3_f33( RGB_w, INPUT_RGB_TO_XYZ );
    
    // Limit
    const float LIMIT_RGB_TO_XYZ[3][3] = RGBtoXYZ_f33( limitingPrimaries, 1.0);
    const float LIMIT_XYZ_TO_RGB[3][3] = XYZtoRGB_f33( limitingPrimaries, 1.0);
    float XYZ_w_limit[3] = mult_f3_f33( RGB_w, LIMIT_RGB_TO_XYZ );

    // Output
    const float OUTPUT_RGB_TO_XYZ[3][3] = RGBtoXYZ_f33( encodingPrimaries, 1.0);
    const float OUTPUT_XYZ_TO_RGB[3][3] = XYZtoRGB_f33( encodingPrimaries, 1.0);
    float XYZ_w_output[3] = mult_f3_f33( RGB_w, OUTPUT_RGB_TO_XYZ );

    // Tables
    const int tableSize = 360;
    
    const ODTParams ODTPARAMS = { 
        peakLuminance,
        100.,                          // reference luminance

        // Tonescale
        TSPARAMS.n_r,                  // normalized white in nits (what 1.0 should be)
        TSPARAMS.g,                    // surround / contrast
        TSPARAMS.t_1,
        TSPARAMS.c_t,
        TSPARAMS.s_2,
        TSPARAMS.u_2,
        TSPARAMS.m_2,
        
        // Chroma Compression
        limitJmax,
        midJ,
        model_gamma,
        sat,
        sat_thr,
        compr,
        
        focusDist,
        
        // Limit
        {{LIMIT_RGB_TO_XYZ[0][0], LIMIT_RGB_TO_XYZ[0][1], LIMIT_RGB_TO_XYZ[0][2]},
         {LIMIT_RGB_TO_XYZ[1][0], LIMIT_RGB_TO_XYZ[1][1], LIMIT_RGB_TO_XYZ[1][2]},
         {LIMIT_RGB_TO_XYZ[2][0], LIMIT_RGB_TO_XYZ[2][1], LIMIT_RGB_TO_XYZ[2][2]}},
        {{LIMIT_XYZ_TO_RGB[0][0], LIMIT_XYZ_TO_RGB[0][1], LIMIT_XYZ_TO_RGB[0][2]},
         {LIMIT_XYZ_TO_RGB[1][0], LIMIT_XYZ_TO_RGB[1][1], LIMIT_XYZ_TO_RGB[1][2]},
         {LIMIT_XYZ_TO_RGB[2][0], LIMIT_XYZ_TO_RGB[2][1], LIMIT_XYZ_TO_RGB[2][2]}},
        {XYZ_w_limit[0],XYZ_w_limit[1],XYZ_w_limit[2]},

        // Output
        {{OUTPUT_RGB_TO_XYZ[0][0], OUTPUT_RGB_TO_XYZ[0][1], OUTPUT_RGB_TO_XYZ[0][2]},
         {OUTPUT_RGB_TO_XYZ[1][0], OUTPUT_RGB_TO_XYZ[1][1], OUTPUT_RGB_TO_XYZ[1][2]},
         {OUTPUT_RGB_TO_XYZ[2][0], OUTPUT_RGB_TO_XYZ[2][1], OUTPUT_RGB_TO_XYZ[2][2]}},
        {{OUTPUT_XYZ_TO_RGB[0][0], OUTPUT_XYZ_TO_RGB[0][1], OUTPUT_XYZ_TO_RGB[0][2]},
         {OUTPUT_XYZ_TO_RGB[1][0], OUTPUT_XYZ_TO_RGB[1][1], OUTPUT_XYZ_TO_RGB[1][2]},
         {OUTPUT_XYZ_TO_RGB[2][0], OUTPUT_XYZ_TO_RGB[2][1], OUTPUT_XYZ_TO_RGB[2][2]}},
        {XYZ_w_output[0],XYZ_w_output[1],XYZ_w_output[2]},
        
        1.14    // lowerHullGamma
    };

    return ODTPARAMS;
}