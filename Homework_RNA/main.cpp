#include <iostream>
#include "RNA.h"
#include <cassert>

void copy(RNA rna){
    std::cout<< "Cardinality of C in func: " << rna.cardinality(C) << "  Len: " << rna.length() << std::endl;
}

int main() {

    // Test atom functions
    size_t * nn = new size_t[4] {0, 1, 2, 3};
    std::cout << "first: " << nn[0] << std::endl;
    size_t ff = get_nucl(65, nn);
    std::cout << "value111: " << ff << std::endl;

    // Test get nucl and set nucl
    ff = get_nucl(64, nn);
    std::cout << "before setting new nucl: " << ff << std::endl;
    set_nucl(64, nn, G);
    ff = get_nucl(64, nn);
    std::cout << "after setting new nucl: " << ff << std::endl;
    std::cout << std::endl;

    // Test cardinality and expansion
    std::cout << "TEST CARDINALITY AND EXPANSION" << std::endl;
    RNA firstRNA = RNA(G, 100);
    std::cout << "Cardinality of G before expansion: " << firstRNA.cardinality(G) << "  Len: " << firstRNA.length() << std::endl;
    firstRNA[10000] = C;
    std::cout<< "Cardinality of G after expansion: " << firstRNA.cardinality(G) << "  Len: " << firstRNA.length() << std::endl;
    std::cout << std::endl;

    std::cout << "TEST CARDINALITY AND assignment( operator= )" << std::endl;
    RNA secondRNA = RNA(C, 10000);
    std::cout<< "Cardinality of C before assignment: " << secondRNA.cardinality(G) << std::endl;
    secondRNA[5] = firstRNA[0];
    std::cout<< "Cardinality of C after assignment: " << secondRNA.cardinality(G) << std::endl;
    std::cout << std::endl;

    std::cout << "TEST ASSIGNMENT( operator=(Nucliotide) )" << std::endl;
    Nucliotide newNucl = Nucliotide(secondRNA[5]);
    std::cout << "Should be C real is " << newNucl << std::endl;
    std::cout << std::endl;

    std::cout << "TEST COMPARATIONS( operator==() )" << std::endl;
    RNA RNA1 = RNA(G, 100);
    RNA RNA2 = RNA(G, 100);
    RNA RNA3 = RNA(T, 100);
    std::cout << "G == G: " << (RNA1 == RNA2) << std::endl;
    std::cout << "G == G: " << (RNA2 == RNA1) << std::endl;
    std::cout << "G != G: " << (RNA1 != RNA2) << std::endl;
    std::cout << "G != G: " << (RNA2 != RNA1) << std::endl;
    std::cout << "G == T: " << (RNA1 == RNA3) << std::endl;
    std::cout << "T == G: " << (RNA3 == RNA1) << std::endl;
    std::cout << "T != G: " << (RNA3 != RNA1) << std::endl;
    std::cout << std::endl;

    std::cout << "TEST COMPARATIONS( operator==() )" << std::endl;
    std::cout<< "Cardinality of G of RNA1 before adding and complimentary: " << RNA1.cardinality(G) << std::endl;
    std::cout<< "Cardinality of T of RNA3 before adding and complimentary: " << RNA3.cardinality(T) << std::endl;
    !RNA1;
    RNA RNA4 = RNA1 + RNA3;
    std::cout<< "Cardinality of C of joined rna : " << RNA4.cardinality(C) << std::endl;
    std::cout<< "Cardinality of T of joined rna: " << RNA4.cardinality(T) << std::endl;
    std::cout << std::endl;

    std::cout << "TEST TRIM" << std::endl;
    RNA RNA5 = RNA(G, 10000);
    std::cout<< "Cardinality before trimed: " << RNA5.cardinality(G) << "  " << RNA5.cardinality(A) << " LEN: " << RNA5.length() << std::endl;
    RNA5.trim(374);
    std::cout<< "Cardinality after trimed: " << RNA5.cardinality(G) << "  " << RNA5.cardinality(A) << " LEN: " << RNA5.length() << std::endl;
    std::cout << std::endl;

    std::cout << "TEST isComplementary" << std::endl;
    RNA RNA_A = RNA(A, 100000);
    RNA RNA_C = RNA(C, 100000);
    RNA RNA_G = RNA(G, 100000);
    RNA RNA_T = RNA(T, 100000);
    std::cout << "Is complimentary A-C: " << RNA_A.isComplementary(RNA_C) << std::endl;
    std::cout << "Is complimentary A-T: " << RNA_A.isComplementary(RNA_T) << std::endl;
    std::cout << "Is complimentary C-G: " << RNA_C.isComplementary(RNA_G) << std::endl;
    std::cout << "Is complimentary C-T: " << RNA_C.isComplementary(RNA_T) << std::endl;

    RNA RNA_A_T = RNA_A + RNA_T;
    RNA RNA_T_A = RNA_T + RNA_A;
    RNA RNA_C_G = RNA_C + RNA_G;
    RNA RNA_G_C = RNA_G + RNA_C;
    std::cout << "Is complimentary AT-TA: " << RNA_A_T.isComplementary(RNA_T_A) << std::endl;
    std::cout << "Is complimentary AT-AT: " << RNA_A_T.isComplementary(RNA_A_T) << std::endl;
    std::cout << "Is complimentary GC-CG: " << RNA_G_C.isComplementary(RNA_C_G) << std::endl;
    std::cout << "Is complimentary CG-CG: " << RNA_C_G.isComplementary(RNA_C_G) << std::endl;

    RNA RNA_A_T_G_C_T_G_A = RNA_A + RNA_T + RNA_G + RNA_C + RNA_T + RNA_G + RNA_A;
    RNA RNA_T_A_C_G_A_C_T = RNA_T + RNA_A + RNA_C + RNA_G + RNA_A + RNA_C + RNA_T;
    RNA RNA_T_A_C_G_A_C_G = RNA_T + RNA_A + RNA_C + RNA_G + RNA_A + RNA_C + RNA_G;

    std::cout << "Is complimentary ATGCTGA-TACGACT: " << RNA_A_T_G_C_T_G_A.isComplementary(RNA_T_A_C_G_A_C_T) << std::endl;
    std::cout << "Is complimentary ATGCTGA-TACGACG: " << RNA_A_T_G_C_T_G_A.isComplementary(RNA_T_A_C_G_A_C_G) << std::endl;
    std::cout << std::endl;


    std::cout << "TEST ASSIGNMENT AND CAPACITY" << std::endl;
    RNA RNAAA = RNA(G, 100);
    size_t allocLength = RNAAA.capacity();
    assert(allocLength >= 2 * 100 / (8 * sizeof(size_t)));
    std::cout<< "Cardinality of C before assignment: " << RNAAA.cardinality(C) << "  Len: " << RNAAA.length() << std::endl;
    RNAAA[1000000] = C;
    RNAAA.cardinality(C);
    std::cout<< "Cardinality of C after assignment: " << RNAAA.cardinality(C) << "  Len: " << RNAAA.length() << std::endl;
    assert(allocLength < RNAAA.capacity());
    std::cout << std::endl;

    RNA const RNAA = RNA(G, 100);
    std::cout << Nucliotide(RNAA[100]) << std::endl;
    RNA RNAA_2 = RNA(G, 1);
    for(size_t i = 0; i < 1000000; ++i)
        RNAA_2[i] = C;

    std::cout << Nucliotide(RNAA_2[999999]) << std::endl;

//
    Nucliotide nucl2 = Nucliotide(RNAA[0]);
    RNAA_2 = RNAA;

    RNAA_2[2] = T;
    RNA tmp_ = RNA(G, 123);
    RNAA_2 = RNA(T, 2423);
    std:: cout << Nucliotide(RNAA_2[100]) << std::endl;

    std:: cout << Nucliotide(tmp_.length()) << std::endl;
    RNAA_2 = std::move(tmp_);
    std:: cout << Nucliotide(RNAA_2.length()) << std::endl;
    std:: cout << Nucliotide(tmp_.length()) << std::endl;

//    RNAA[2] = C;


    // const
    // move
    // =
    // []
    //for 1 nucl

    return 0;
}
