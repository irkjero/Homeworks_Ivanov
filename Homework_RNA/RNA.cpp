//
// Created by Петр on 04.10.2020.
//
#include <iostream>

#include "RNA.h"


struct LengthError : public std::exception {
    const char * what () const throw () {
        return "Len of RNAs are different, but should be the same for comparing";
    }
};


struct IndexError : public std::exception {
    const char * what () const throw () {
        return "Index out of a range of RNA length";
    }
};


RNA::RNA (const RNA & rna) : len_rna{rna.len_rna}{
    auto tmp_nucls = new size_t[rna.capacity()];
    for(size_t i = 0; i < rna.len_rna; i++){
        auto tmp_nucl = static_cast<Nucliotide >(get_nucl(i, rna.nucls));
        set_nucl(i, tmp_nucls, tmp_nucl);
    }
    if(nucls != nullptr){
        delete[] nucls;
    }
    nucls = tmp_nucls;
}


RNA::RNA (RNA && rna): nucls{rna.nucls}, len_rna{rna.len_rna} {
    rna.nucls = nullptr;
    rna.len_rna = 0;
}


RNA::RNA (size_t * nucls_tmp, size_t new_len) : nucls{nucls_tmp}, len_rna{new_len} {
    nucls_tmp = nullptr;
}


RNA::RNA(Nucliotide nucl, size_t len){
    size_t tmp_len = (2 * len) / (sizeof(len) * 8);
    nucls = new size_t[tmp_len + 1] {0};
    for(size_t i = 0; i < len; i++) {
        set_nucl(i, nucls, nucl);
    }
    len_rna = len;
}


bool RNA::operator== (RNA const & rna2) const{
    if(length() != rna2.length())
        throw LengthError();
    for(size_t i = 0; i < length(); i++)
        if(get_nucl(i, nucls) != get_nucl(i, rna2.nucls))
            return false;
    return true;
}


bool RNA::operator!= (RNA const & rna2) const{
    return !(*this == rna2);
}


bool isCompl(size_t nucl_1, size_t nucl_2){
    nucl_1 = static_cast<Nucliotide >(nucl_1);
    nucl_2 = static_cast<Nucliotide >(nucl_2);
    if((nucl_1 == A and nucl_2 == T) or (nucl_1 == T and nucl_2 == A)){
        return true;
    }else if((nucl_1 == G and nucl_2 == C) or (nucl_1 == C and nucl_2 == G)){
        return true;
    }
    return false;
}


bool RNA::isComplementary(RNA &amp) const{
    if(length() != amp.length())
        throw LengthError();
    for(size_t i = 0; i < length(); i++){
        size_t nucl_1 = get_nucl(i, nucls);
        size_t nucl_2 = get_nucl(i, amp.nucls);

        nucl_1 = nucl_1 & static_cast<size_t >(3);
        nucl_2 = nucl_2 & static_cast<size_t >(3);
        if(! isCompl(nucl_1, nucl_2)){
            return false;
        }
    }
    return true;
}


RNA operator+(const RNA & r1, const RNA & r2){
    size_t new_capacity = (r1.length() + r2.length()) / (8 * sizeof(std::size_t) / 2);
    auto * new_nucls = new size_t[new_capacity + 1];
    for(size_t i = 0; i < r1.length(); i++)
        set_nucl(i, new_nucls, static_cast<Nucliotide >(get_nucl(i, r1.nucls)));
    for(size_t i = r1.length() ; i < r2.length() + r1.length(); i++)
        set_nucl(i, new_nucls, static_cast<Nucliotide >(get_nucl(i - r1.length(), r2.nucls)));

    return RNA(new_nucls, r1.length() + r2.length());
}


RNA::Reference RNA::operator[](size_t num_element){
    size_t new_len = 2 * (num_element + 1); //num_element + 1;
    if(len_rna < num_element + 1){
        size_t tmp_len = (2 * new_len) / (sizeof(new_len) * 8);
        auto * tmp_nucl = new size_t[tmp_len + 1] {0};
        for(size_t i = 0; i < len_rna; i++)
            set_nucl(i, tmp_nucl, static_cast<Nucliotide >(get_nucl(i, nucls)));
        delete[] nucls;
        nucls = tmp_nucl;
        tmp_nucl = nullptr;
        len_rna = new_len;
    }
    return Reference(this, num_element);
}


const RNA::Reference RNA::operator[](size_t num_element) const{
    return Reference(this, num_element);;
}



void RNA::operator!(){
    std::unordered_map<Nucliotide, Nucliotide > complimentary = {
            {A, T}, {T, A}, {C, G}, {G, C}
    };
    for(size_t i = 0; i < length(); i++){
        auto compl_nucl = static_cast<Nucliotide>(get_nucl(i, nucls));
        compl_nucl = complimentary[compl_nucl];
        set_nucl(i, nucls, compl_nucl);
    }
}


size_t RNA::capacity() const{
    return len_rna / (8 * sizeof(std::size_t) / 2) + 1;
}


std::unordered_map<Nucliotide, size_t, std::hash<size_t>> RNA::cardinality() const{
    std::unordered_map<Nucliotide, size_t, std::hash<size_t> > card;
    for(size_t i = 0; i < len_rna; i++){
            size_t nucl_2 = get_nucl(i, nucls);
            Nucliotide casted = static_cast<Nucliotide>(nucl_2);
            if(card.count(casted))
                card[casted] += 1;
            else
                card[casted] = 1;
        }
    return card;
}


size_t RNA::cardinality(Nucliotide nucl) const{
    std::unordered_map<Nucliotide, size_t, std::hash<size_t> > card = cardinality();
    return card[nucl];
}


void RNA::trim(size_t lastIndex){
    size_t new_len = lastIndex + 1;
    size_t new_cap = (2 * new_len) / (8 * sizeof(std::size_t));
    size_t *tmp_nucls = new size_t[new_cap + 1];
    for(size_t i = 0; i < new_len; i++){
        auto tmp_nucl = static_cast<Nucliotide >(get_nucl(i, nucls));
        set_nucl(i, tmp_nucls, tmp_nucl);
    }
    delete [] nucls;
    nucls = tmp_nucls;
    len_rna = new_len;
}


std::pair<RNA, RNA> RNA::split(size_t index){
    if(len_rna <= index){
        throw IndexError();
    }

    RNA rna1 = RNA(A, index);
    RNA rna2 = RNA(A, len_rna - index);

    for(size_t i = 0; i < len_rna; ++i){
        if(i < index){
            set_nucl(i, rna1.nucls, Nucliotide(get_nucl(i, nucls)));
        }
        else{
            set_nucl(i - index, rna2.nucls, Nucliotide(get_nucl(i, nucls)));
        }
    }

    rna1.len_rna = index;
    rna2.len_rna = len_rna - index;

    return std::pair<RNA, RNA>(rna1, rna2);
}


size_t RNA::length() const{
    return len_rna;
}


RNA& RNA::operator=(const RNA& rna){
    if(this == &rna){
        return *this;
    }

    if(nucls)
        delete[] nucls;
    len_rna = rna.len_rna;
    size_t capacity_new = rna.capacity();
    nucls = new size_t[capacity_new];
    for(size_t i = 0; i < capacity_new; ++i)
        nucls[i] = rna.nucls[i];
    return *this;
}


//RNA& RNA::operator=(RNA&& rna) noexcept{
//    if(this == &rna){
//        return *this;
//    }
//
//    if(nucls)
//        delete[] nucls;
//
//    nucls = rna.nucls;
//    len_rna = rna.len_rna;
//    rna.nucls = nullptr;
//    rna.len_rna = 0;
//
//    return *this;
//}


RNA::~RNA(){
    if(nucls != nullptr){
        delete[] nucls;
    }
}


size_t modifyBit(size_t n, size_t p, size_t b)
{
    size_t mask = static_cast<size_t>(1) << p;
    return (n & ~mask) | ((b << p) & mask);
}


size_t get_nucl_(size_t N, std::size_t nucls){
    std::size_t mask = 3;
    std::size_t step = 2 * N;
    std::size_t masked_n =  nucls >> step;
    std::size_t bit = masked_n & mask;
    return bit;
}

size_t get_nucl(size_t N, std::size_t* nucls){
    size_t cell_num = N / (8 * sizeof(std::size_t) / 2);
    std::size_t cell = nucls[cell_num];
    return get_nucl_(N - (cell_num * (8 * sizeof(std::size_t) / 2)), cell);
}


size_t set_nucl_(size_t N, std::size_t nucls, std::size_t nucl){
    std::size_t first_bit = nucl & static_cast<size_t>(1);
    std::size_t second_bit = (nucl >> static_cast<size_t>(1)) & static_cast<size_t>(1);
    nucls = modifyBit(nucls, 2 * N + 1, second_bit);
    nucls = modifyBit(nucls, 2 * N , first_bit);
    return nucls;
}

size_t set_nucl(size_t N, std::size_t* nucls, Nucliotide new_nucl){
    size_t cell_num = N / (8 * sizeof(std::size_t) / 2);
    std::size_t cell = nucls[cell_num];
    nucls[cell_num] = set_nucl_(N - (cell_num * (8 * sizeof(std::size_t) / 2)), cell, std::size_t(new_nucl));
    return 0;
}