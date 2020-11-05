//
// Created by Петр on 04.10.2020.
//

#ifndef HOMEWORK_RNA_RNA_H
#define HOMEWORK_RNA_RNA_H
#include <unordered_map>
#include "Nucl.h"


size_t modifyBit(size_t n, size_t p, size_t b);

size_t get_nucl_(size_t N, std::size_t nucls);

size_t get_nucl(size_t N, std::size_t* nucls);

size_t set_nucl_(size_t N, std::size_t nucls, std::size_t nucl);

size_t set_nucl(size_t N, std::size_t* nucls, Nucliotide new_nucl);


class RNA {
    size_t * nucls;
    size_t len_rna;
public:
    class Reference{
        RNA *rna;
        size_t number;
        public:
            Reference(RNA *rna_, size_t number_): rna{rna_}, number{number_} {}
            Reference( const RNA *rna_, size_t number_): rna{const_cast<RNA *>(rna_)}, number{number_} {}

            explicit operator Nucliotide(){
                return static_cast<Nucliotide>(get_nucl(number, rna->nucls));
            }

            explicit operator Nucliotide() const{
                return static_cast<const Nucliotide>(get_nucl(number, rna->nucls));
        }

            Reference & operator=(Nucliotide new_nucl){
                set_nucl(number, rna->nucls, new_nucl);
                return *this;
            }

            Reference & operator=(Reference const & new_ref){
                auto new_nucl = static_cast<Nucliotide>(get_nucl(new_ref.number, new_ref.rna->nucls));
                set_nucl(number, rna->nucls, new_nucl);
                return *this;
            }
    };

    RNA(Nucliotide nucl, size_t len);
    RNA (const RNA & rna);
    RNA (RNA && rna);
    RNA (size_t * nucls_tmp, size_t new_len);

    Reference operator[](size_t num_element);
    const Reference operator[](size_t num_element) const;
    bool operator== (RNA const & rna2) const;
    bool operator!= (RNA const & rna2) const;
    void operator!();
    friend RNA operator+(const RNA & r1, const RNA & r2);
    RNA& operator=(const RNA&);
//    RNA& operator=(RNA&&) noexcept;
    template <class T>
    T& operator=(T&& rna){
        if(this == &rna){
            return *this;
        }
        RNA tmp(std::forward<T>(rna));
        std::swap(tmp.nucls, nucls);
        std::swap(tmp.len_rna, len_rna);

        return *this;
    }

    bool isComplementary (RNA &amp) const;
    std::pair<RNA, RNA> split(size_t index);
    size_t capacity() const;
    size_t cardinality(Nucliotide nucl) const;
    std::unordered_map<Nucliotide, size_t, std::hash<size_t>> cardinality() const;
    void trim(size_t lastIndex);
    size_t length() const;

    ~RNA();

};

#endif //HOMEWORK_RNA_RNA_H
