## Matrix Algebra in CTL ##

### Summary of Differences ###

In CTL, the order of operands in vector-times-matrix functions is swapped from "textbook" convention. Textbooks often use “matrix-times-column vector” notation, but CTL functions expect “row vector-times-matrix” syntax. 

(See page 40-41 of [CtlManual.pdf](https://github.com/ampas/CTL/tree/master/doc/CtlManual.pdf) for more information )

To convert textbook notation to row vector-times-matrix notation requires transposing all matrices and swapping the left and right operands of multiplications.

#### Textbook notation ####

Textbooks write the operation of a matrix times a vector as:

    [A][B] = [C]

    where A is a '3 row by 3 column' matrix (3x3)
          B is a '3 row by 1 column' vector (3x1)
          C is a '3 row by 1 column' vector (3x1)

With example values provided for `[A]`, and in the context of RGB color vectors for `[B]` and `[C]`, this becomes:

     _         _   _      _     _       _
    |  1  2  3  | |  R_in  |   |  R_out  |
    |  4  5  6  | |  G_in  | = |  G_out  |
    |_ 7  8  9 _| |_ B_in _|   |_ B_out _|

which, when written out fully is equivalent to:

    R_out =  1 * R_in +  2 * G_in +  3 * B_in;
    G_out =  4 * R_in +  5 * G_in +  6 * B_in;
    B_out =  7 * R_in +  8 * G_in +  9 * B_in;

#### CTL notation ####
In CTL, the same operation would be:

    const float A[3][3] =                 // This is a 3 row by 3 column vector (3x3)
    {                                     // * Note that it is transposed from 
      { 1,  4,  7},                       //   the textbook representation.
      { 2,  5,  8},
      { 3,  6,  9}
    };  
    
    float B[3] = { R_in, G_in, B_in };    // This is a 1 row by 3 column vector (1x3)
    
    float C[3] = mult_f3_f33( B, A);      // Results in a 1 row by 3 column vector (1x3)
    
    float R_out = C[0];
    float G_out = C[1];
    float B_out = C[2];



### Values for ACES Transforms ###
In the ACES tranforms, built-in CTL functions are used for vector/matrix operations. 

For absolute clarity of the math being performed, the name of a matrix as it appears in the CTL transforms is listed below, followed by the explicit math that would calculate the same result as the built in functions when using that matrix as the operator.

Note that the values of the matrix coefficients are rounded to 10 decimal places, but in actual implementations, indexing into stored variables at full precision of the specific implementation is preferred.



#### AP0-to-XYZ Matrix ####
In the code, matrix is created by:

    const float AP0_2_XYZ_MAT[4][4] = RGBtoXYZ( AP0, 1.0);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, AP0_2_XYZ_MAT);

Equivalent math:

    R_out =  0.9525523959 * R_in +  0.0000000000 * G_in +  0.0000936786 * B_in;
    G_out =  0.3439664498 * R_in +  0.7281660966 * G_in + -0.0721325464 * B_in;
    B_out =  0.0000000000 * R_in +  0.0000000000 * G_in +  1.0088251844 * B_in;

#### XYZ-to-AP0 Matrix ####
In the code, matrix is created by:

    const float XYZ_2_AP0_MAT[4][4] = XYZtoRGB( AP0, 1.0);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, XYZ_2_AP0_MAT);

Equivalent math:

    R_out =  1.0498110175 * R_in +  0.0000000000 * G_in + -0.0000974845 * B_in;
    G_out = -0.4959030231 * R_in +  1.3733130458 * G_in +  0.0982400361 * B_in;
    B_out =  0.0000000000 * R_in +  0.0000000000 * G_in +  0.9912520182 * B_in;
    
#### AP1-to-XYZ Matrix ####
In the code, matrix is created by:

    const float AP1_2_XYZ_MAT[4][4] = RGBtoXYZ( AP1, 1.0);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, AP1_2_XYZ_MAT);

Equivalent math:

    R_out =  0.6624541811 * R_in +  0.1340042065 * G_in +  0.1561876870 * B_in;
    G_out =  0.2722287168 * R_in +  0.6740817658 * G_in +  0.0536895174 * B_in;
    B_out = -0.0055746495 * R_in +  0.0040607335 * G_in +  1.0103391003 * B_in;

#### XYZ-to-AP1 Matrix ####
In the code, matrix is created by:

    const float XYZ_2_AP1_MAT[4][4] = XYZtoRGB( AP1, 1.0);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, XYZ_2_AP1_MAT);

Equivalent math:

    R_out =  1.6410233797 * R_in + -0.3248032942 * G_in + -0.2364246952 * B_in;
    G_out = -0.6636628587 * R_in +  1.6153315917 * G_in +  0.0167563477 * B_in;
    B_out =  0.0117218943 * R_in + -0.0082844420 * G_in +  0.9883948585 * B_in;    

#### AP0-to-AP1 Matrix ####
In the code, matrix is created by:

    const float AP0_2_AP1_MAT[4][4] = mult_f44_f44( AP0_2_XYZ_MAT, XYZ_2_AP1_MAT);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, AP0_2_AP1_MAT);

Equivalent math:

    R_out =  1.4514393161 * R_in + -0.2365107469 * G_in + -0.2149285693 * B_in;
    G_out = -0.0765537734 * R_in +  1.1762296998 * G_in + -0.0996759264 * B_in;
    B_out =  0.0083161484 * R_in + -0.0060324498 * G_in +  0.9977163014 * B_in;

#### AP1-to-AP0 Matrix ####
In the code, matrix is created by:

    const float AP1_2_AP0_MAT[4][4] = mult_f44_f44( AP1_2_XYZ_MAT, XYZ_2_AP0_MAT);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, AP1_2_AP0_MAT);

Equivalent math:

    R_out =  0.6954522414 * R_in +  0.1406786965 * G_in +  0.1638690622 * B_in;
    G_out =  0.0447945634 * R_in +  0.8596711185 * G_in +  0.0955343182 * B_in;
    B_out = -0.0055258826 * R_in +  0.0040252103 * G_in +  1.0015006723 * B_in;

#### XYZ-to-P3D60 Primaries Matrix ####
In the code, matrix is created by:

    const float XYZ_2_P3D60_PRI_MAT[4][4] = XYZtoRGB( P3D60_PRI, 1.0);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, XYZ_2_P3D60_PRI_MAT);

Equivalent math:

    R_out =  2.4027414142 * R_in + -0.8974841639 * G_in + -0.3880533700 * B_in;
    G_out = -0.8325796487 * R_in +  1.7692317536 * G_in +  0.0237127115 * B_in;
    B_out =  0.0388233815 * R_in + -0.0824996856 * G_in +  1.0363685997 * B_in;

#### P3D60-to-XYZ Matrix ####
In the code, matrix is created by:

    const float P3D60_2_XYZ_MAT[4][4] = RGBtoXYZ( P3D60_PRI, 1.0);

and the operator would be:
  
    const float XYZ_out[3] = mult_f3_f44( RGB_in, P3D60_2_XYZ_PRI_MAT);

Equivalent math:

	X_out =  0.5049495342 * R_in +  0.2646814889 * G_in +  0.1830150515 * B_in;
	Y_out =  0.2376233102 * R_in +  0.6891706692 * G_in +  0.0732060206 * B_in;
	Z_out =  0.0000000000 * R_in +  0.0449459132 * G_in +  0.9638792711 * B_in;

#### XYZ-to-P3DCI Primaries Matrix ####
In the code, matrix is created by:

    const float XYZ_2_P3DCI_PRI_MAT[4][4] = XYZtoRGB( P3DCI_PRI, 1.0);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, XYZ_2_P3DCI_PRI_MAT);

Equivalent math:

    R_out =  2.7253940305 * R_in + -1.0180030062 * G_in + -0.4401631952 * B_in;
    G_out = -0.7951680258 * R_in +  1.6897320548 * G_in +  0.0226471906 * B_in;
    B_out =  0.0412418914 * R_in + -0.0876390192 * G_in +  1.1009293786 * B_in;

#### XYZ-to-REC709 Primaries Matrix ####
In the code, matrix is created by:

    const float XYZ_2_REC709_PRI_MAT[4][4] = XYZtoRGB( REC709_PRI, 1.0);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, XYZ_2_REC709_PRI_MAT);

Equivalent math:

    R_out =  3.2409699419 * R_in + -1.5373831776 * G_in + -0.4986107603 * B_in;
    G_out = -0.9692436363 * R_in +  1.8759675015 * G_in +  0.0415550574 * B_in;
    B_out =  0.0556300797 * R_in + -0.2039769589 * G_in +  1.0569715142 * B_in;

#### XYZ-to-REC2020 Primaries Matrix ####
In the code, matrix is created by:

    const float XYZ_2_REC2020_PRI_MAT[4][4] = XYZtoRGB( REC2020_PRI, 1.0);

and the operator would be:
  
    const float RGB_out[3] = mult_f3_f44( RGB_in, XYZ_2_REC2020_PRI_MAT);

Equivalent math:

    R_out =  1.7166511880 * R_in + -0.3556707838 * G_in + -0.2533662814 * B_in;
    G_out = -0.6666843518 * R_in +  1.6164812366 * G_in +  0.0157685458 * B_in;
    B_out =  0.0176398574 * R_in + -0.0427706133 * G_in +  0.9421031212 * B_in;

#### Bradford Chromatic Adaptation Matrix from D60 to D65 ####
In the code, matrix is created by:

    const float CAT_MAT[3][3] = calculate_cat_matrix( ACES_PRI.white, REC709_PRI.white);

and the operator would be:
  
    const float XYZ_out[3] = mult_f3_f44( XYZ_in, CAT_MAT);

Equivalent math:

    X_out =  0.987224   * X_in + -0.00611327 * Y_in +  0.0159533  * Z_in;
    Y_out = -0.00759836 * X_in +  1.00186    * Y_in +  0.00533002 * Z_in;
    Z_out =  0.00307257 * X_in + -0.00509595 * Y_in +  1.08168    * Z_in;