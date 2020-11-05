#include <iostream>
#include "RNA.h"
#include <cassert>
#include <gtest/gtest.h>

 
TEST(AtomaryFanctions, GetNucl) { 
    size_t * nn = new size_t[4] {0, 1, 2, 3};
    size_t ff = get_nucl(64, nn);
    
    ASSERT_EQ(2, ff);
    
    ff = get_nucl(0, nn);
    ASSERT_EQ(0, ff);
    
    ff = get_nucl(32, nn);
    ASSERT_EQ(1, ff);
}

 
TEST(AtomaryFanctions, SetNucl) {
	size_t * nn = new size_t[4] {0, 1, 2, 3};
    size_t ff = get_nucl(64, nn);
    
    set_nucl(43, nn, G);
    ff = get_nucl(43, nn);
    
    ASSERT_EQ(size_t(G), ff);
    
    set_nucl(47, nn, C);
    ff = get_nucl(47, nn);
    
    ASSERT_EQ(size_t(C), ff);
}


TEST(MethodsRNA, Cardinality) {
    RNA firstRNA = RNA(G, 1000);
    firstRNA[999] = C;
    
    ASSERT_EQ(999, firstRNA.cardinality(G));
    ASSERT_EQ(1, firstRNA.cardinality(C));
	
}


TEST(MethodsRNA, Length) {
    RNA firstRNA = RNA(G, 10000);
    firstRNA[1000] = C;
    RNA secondRNA = RNA(G, 1000000);
    
    ASSERT_EQ(10000, firstRNA.length());
    ASSERT_EQ(1000000, secondRNA.length());
    
    firstRNA[10000000] = T;
    ASSERT_EQ(20000002, firstRNA.length());
    
}


TEST(OperatorsRNA, Assignment) {
	RNA firstRNA = RNA(T, 10000);
    Nucliotide newNucl = Nucliotide(firstRNA[5]);
    ASSERT_EQ(size_t(T), size_t(newNucl));

    RNA const RNAA = RNA(G, 100);
    ASSERT_EQ(size_t(G), size_t(Nucliotide(RNAA[99])));
    
    RNA RNAA_2 = RNA(G, 1);
    
}


TEST(OperatorsRNA, PushBack) {
	RNA firstRNA = RNA(T, 10);
    for(size_t i = 0; i < 10000000; ++i)
        firstRNA[i] = C;

    ASSERT_EQ(size_t(C), size_t(Nucliotide(firstRNA[9999999])));
    
}


TEST(OperatorsRNA, Comapations) {
	RNA RNA1 = RNA(G, 1000);
    RNA RNA2 = RNA(G, 1000);
    RNA RNA3 = RNA(T, 1000);
    
    ASSERT_TRUE(RNA1 == RNA2);
    ASSERT_TRUE(RNA2 == RNA1);
    ASSERT_FALSE(RNA1 != RNA2);
    ASSERT_FALSE(RNA2 != RNA1);
    ASSERT_FALSE(RNA1 == RNA3);
    ASSERT_FALSE(RNA3 == RNA1);
    ASSERT_TRUE(RNA3 != RNA1);
    
}


TEST(OperatorsRNA, Sum) {
	RNA RNA1 = RNA(G, 1000);
    RNA RNA3 = RNA(T, 1000);
    
    !RNA1;
    RNA RNA4 = RNA1 + RNA3;

    ASSERT_EQ(1000, RNA4.cardinality(C));
    ASSERT_EQ(1000, RNA4.cardinality(T));
    ASSERT_EQ(0, RNA4.cardinality(A));
    ASSERT_EQ(0, RNA4.cardinality(G));

    
}


TEST(MethodsRNA, Trim) {
    RNA RNA5 = RNA(G, 10000);
    ASSERT_EQ(10000, RNA5.cardinality(G));
    ASSERT_EQ(10000, RNA5.length());
    RNA5.trim(374);
    ASSERT_EQ(375, RNA5.cardinality(G));
    ASSERT_EQ(375, RNA5.length());
    
}


TEST(MethodsRNA, IsComplementary) {
    RNA RNA_A = RNA(A, 100000);
    RNA RNA_C = RNA(C, 100000);
    RNA RNA_G = RNA(G, 100000);
    RNA RNA_T = RNA(T, 100000);
    
    ASSERT_FALSE(RNA_A.isComplementary(RNA_C));
    ASSERT_TRUE(RNA_A.isComplementary(RNA_T));
    ASSERT_TRUE(RNA_C.isComplementary(RNA_G));
    ASSERT_FALSE(RNA_C.isComplementary(RNA_T));
    

    RNA RNA_A_T = RNA_A + RNA_T;
    RNA RNA_T_A = RNA_T + RNA_A;
    RNA RNA_C_G = RNA_C + RNA_G;
    RNA RNA_G_C = RNA_G + RNA_C;
    
    ASSERT_TRUE(RNA_A_T.isComplementary(RNA_T_A));
    ASSERT_FALSE(RNA_A_T.isComplementary(RNA_A_T));
    ASSERT_TRUE(RNA_G_C.isComplementary(RNA_C_G));
    ASSERT_FALSE(RNA_C_G.isComplementary(RNA_C_G));
   

    RNA RNA_A_T_G_C_T_G_A = RNA_A + RNA_T + RNA_G + RNA_C + RNA_T + RNA_G + RNA_A;
    RNA RNA_T_A_C_G_A_C_T = RNA_T + RNA_A + RNA_C + RNA_G + RNA_A + RNA_C + RNA_T;
    RNA RNA_T_A_C_G_A_C_G = RNA_T + RNA_A + RNA_C + RNA_G + RNA_A + RNA_C + RNA_G;
    
    ASSERT_TRUE(RNA_A_T_G_C_T_G_A.isComplementary(RNA_T_A_C_G_A_C_T));
    ASSERT_FALSE(RNA_A_T_G_C_T_G_A.isComplementary(RNA_T_A_C_G_A_C_G));
    
}

 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
