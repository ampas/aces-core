void
function3
    (input varying half vh1,
     input varying float vf1,
     input varying unsigned vu1,

     input uniform Box2i box2i1,
     input uniform Box2f box2f1,
     input uniform Chromaticities chr1,
     input uniform float d1,
     input uniform float f1,
     input uniform int i1,
     input uniform float m33f1[3][3],
     input uniform float m44f1[4][4],
     input uniform float v2f1[2],
     input uniform int v2i1[2],
     input uniform float v3f1[3],
     input uniform int v3i1[3],

     output varying half vh2,
     output varying float vf2,
     output varying unsigned vu2,

     output uniform Box2i box2i2,
     output uniform Box2f box2f2,
     output uniform Chromaticities chr2,
     output uniform float d2,
     output uniform float f2,
     output uniform int i2,
     output uniform float m33f2[3][3],
     output uniform float m44f2[4][4],
     output uniform float v2f2[2],
     output uniform int v2i2[2],
     output uniform float v3f2[3],
     output uniform int v3i2[3])
{
     vh2 = vh1;
     vf2 = vf1;
     vu2 = vu1;

     box2i2 = box2i1;
     box2f2 = box2f1;
     chr2 = chr1;
     d2 = d1;
     f2 = f1;
     i2 = i1;
     m33f2 = m33f1;
     m44f2 = m44f1;
     v2f2 = v2f1;
     v2i2 = v2i1;
     v3f2 = v3f1;
     v3i2 = v3i1;
}
