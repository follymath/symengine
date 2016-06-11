#include "catch.hpp"
#include <iostream>

#include <symengine/basic.h>
#include <symengine/fields.h>
#include <symengine/symengine_rcp.h>
#include <symengine/dict.h>
#include <symengine/symbol.h>

using SymEngine::RCP;
using SymEngine::Symbol;
using SymEngine::symbol;
using SymEngine::gf_poly;
using SymEngine::integer_class;
using SymEngine::map_uint_mpz;
using SymEngine::GaloisField;
using SymEngine::GaloisFieldDict;

using namespace SymEngine::literals;

TEST_CASE("Constructor of GaloisField : Basic", "[basic]")
{
    RCP<const Symbol> x = symbol("x");
    RCP<const GaloisField> P = gf_poly(x, {{0, 1_z}, {1, 2_z}, {2, 1_z}}, 2_z);
    REQUIRE(P->__str__() == "x**2 + 1");

    RCP<const GaloisField> Q = GaloisField::from_vec(x, {1_z, 0_z, 2_z, 3_z}, 2_z);
    REQUIRE(Q->__str__() == "x**3 + 1");

    RCP<const GaloisField> R = GaloisField::from_vec(x, {17_z, 0_z, 7_z, 9_z}, 7_z);
    REQUIRE(R->__str__() == "2*x**3 + 3");

    RCP<const GaloisField> S = gf_poly(x, {{0, 2_z}}, 7_z);
    REQUIRE(S->__str__() == "2");

    RCP<const GaloisField> T = gf_poly(x, map_uint_mpz{}, 7_z);
    REQUIRE(T->__str__() == "0");

    RCP<const GaloisField> U = gf_poly(x, {{0, 2_z}, {1, 0_z}, {2, 0_z}}, 7_z);
    REQUIRE(U->__str__() == "2");
}

TEST_CASE("GaloisField Addition, Subtraction, Multiplication : Basic", "[basic]")
{
    RCP<const Symbol> x = symbol("x");
    std::vector<integer_class> a = {2_z, 3_z, 4_z};
    std::vector<integer_class> b = {3_z, 3_z, 6_z, 6_z};
    RCP<const GaloisField> r1 = GaloisField::from_vec(x, a, 5_z);
    RCP<const GaloisField> r2 = GaloisField::from_vec(x, b, 5_z);
    RCP<const GaloisField> r3 = add_upoly(*r1, *r2);
    REQUIRE(r3->__str__() == "x**3 + x");
    r3 = sub_upoly(*r1, *r2);
    REQUIRE(r3->__str__() == "4*x**3 + 3*x**2 + 4");
    r3 = mul_upoly(*r1, *r2);
    auto mp = r3->get_dict();
    REQUIRE(mp[0] == 1);
    REQUIRE(mp[2] == 3);
    REQUIRE(mp[3] == 2);
    REQUIRE(mp[4] == 2);
    REQUIRE(mp[5] == 4);

    a = {};
    r1 = GaloisField::from_vec(x, a, 11_z);
    r2 = neg_upoly(*r1);
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    r3 = GaloisField::from_vec(x, {0_z}, 11_z);
    r2 = add_upoly(*r1, *r3);
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    r2 = sub_upoly(*r1, *r3);
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    r2 = mul_upoly(*r1, *r3);
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    
    r3 = GaloisField::from_vec(x, {3_z}, 11_z);
    r2 = add_upoly(*r1, *r3);
    REQUIRE(r2->__str__() == "3");
    r2 = sub_upoly(*r1, *r3);
    REQUIRE(r2->__str__() == "8");
    r2 = mul_upoly(*r1, *r3);
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    r2 = GaloisField::from_vec(x, a, 11_z);
    a = {2_z};
    r2 = GaloisField::from_vec(x, a, 11_z);
    r3 = add_upoly(*r1, *r2);
    REQUIRE(r2->__str__() == "2");
    r3 = sub_upoly(*r1, *r2);
    r3 = sub_upoly(*r1, *r2);
    REQUIRE(r3->__str__() == "9");
    r2 = quo_upoly(*r1, *GaloisField::from_vec(x, {3_z}, 11_z));
    mp = r2->get_dict();
    REQUIRE(mp.empty());

    a = {1_z};
    r1 = GaloisField::from_vec(x, a, 11_z);
    r2 = neg_upoly(*r1);
    REQUIRE(r2->__str__() == "10");
    r2 = add_upoly(*r1, *GaloisField::from_vec(x, {10_z}, 11_z));
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    r2 = add_upoly(*r1, *GaloisField::from_vec(x, {4_z}, 11_z));
    mp = r2->get_dict();
    REQUIRE(r2->__str__() == "5");
    r2 = sub_upoly(*r1, *GaloisField::from_vec(x, {1_z}, 11_z));
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    r2 = sub_upoly(*r1, *GaloisField::from_vec(x, {4_z}, 11_z));
    REQUIRE(r2->__str__() == "8");
    r2 = mul_upoly(*r1, *GaloisField::from_vec(x, {}, 11_z));
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    r2 = mul_upoly(*r1, *GaloisField::from_vec(x, {3_z}, 11_z));
    REQUIRE(r2->__str__() == "3");
    r2 = add_upoly(*r1, *GaloisField::from_vec(x, {3_z}, 11_z));
    mp = r2->get_dict();
    REQUIRE(r2->__str__() == "4");

    a = {1_z, 2_z, 3_z};
    r1 = GaloisField::from_vec(x, a, 11_z);
    r2 = neg_upoly(*r1);
    REQUIRE(r2->__str__() == "8*x**2 + 9*x + 10");
    r2 = add_upoly(*r1, *GaloisField::from_vec(x, {4_z}, 11_z));
    REQUIRE(r2->__str__() == "3*x**2 + 2*x + 5");
    r2 = sub_upoly(*r1, *GaloisField::from_vec(x, {1_z}, 11_z));
    REQUIRE(r2->__str__() == "3*x**2 + 2*x");
    r2 = mul_upoly(*r1, *GaloisField::from_vec(x, {}, 11_z));
    mp = r2->get_dict();
    REQUIRE(mp.empty());
    r2 = mul_upoly(*r1, *GaloisField::from_vec(x, {7_z}, 11_z));
    mp = r2->get_dict();
    REQUIRE(mp[0] == 7);
    REQUIRE(mp[1] == 3);
    REQUIRE(mp[2] == 10);

    a = {3_z, 2_z, 1_z};
    b = {8_z, 9_z, 10_z};
    r1 = GaloisField::from_vec(x, a, 11_z);
    r2 = GaloisField::from_vec(x, b, 11_z);
    r3 = sub_upoly(*r1, *r2);
    REQUIRE(r3->__str__() == "2*x**2 + 4*x + 6");
    a = {3_z, 0_z, 0_z, 6_z, 1_z, 2_z};
    b = {4_z, 0_z, 1_z, 0_z};
    r1 = GaloisField::from_vec(x, a, 11_z);
    r2 = GaloisField::from_vec(x, b, 11_z);
    mp = mul_upoly(*r2, *r1)->get_dict();
    REQUIRE(mp[0] == 1);
    REQUIRE(mp[1] == 0);
    REQUIRE(mp[2] == 3);
    REQUIRE(mp[3] == 2);
    REQUIRE(mp[4] == 4);
    REQUIRE(mp[5] == 3);
    REQUIRE(mp[6] == 1);
    REQUIRE(mp[7] == 2);
    REQUIRE(mp[8] == 0);
}
TEST_CASE("GaloisFieldDict Division, GCD, LCM, Shifts : Basic", "[basic]")
{
    std::vector<integer_class> a, b;
    map_uint_mpz mp;
    GaloisFieldDict d1, d2, d3, d4;
    a = {0_z, 1_z, 2_z, 3_z, 4_z, 5_z};
    b = {0_z, 3_z, 2_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 7_z);
    d2 = GaloisFieldDict::from_vec(b, 7_z);
    d1.gf_div(d2, outArg(d3), outArg(d4));
    mp = d3.get_dict();
    REQUIRE(mp[0] == 0);
    REQUIRE(mp[1] == 1);
    REQUIRE(mp[2] == 5);
    mp = d4.get_dict();
    REQUIRE(mp[0] == 0);
    REQUIRE(mp[1] == 1);
    REQUIRE(mp[2] == 6);
    REQUIRE(d3 == d1/d2);

    d2 = d1;
    d2 *= 2_z;
    d1 += d1;
    REQUIRE(d1 == d2);
    d2 = d1;
    d2 = d1.gf_sqr();
    d1 *= d1;
    REQUIRE(d1 == d2);
    a = {};
    d1 = GaloisFieldDict::from_vec(a, 7_z);
    d2 -= d2;
    REQUIRE(d2.dict_.empty());
    d1 = GaloisFieldDict::from_vec({1_z}, 7_z);
    d2 = GaloisFieldDict::from_vec(b, 7_z);
    d2 /= d2;
    REQUIRE(d1 == d2);

    a = {0_z, 1_z, 2_z, 3_z, 4_z, 5_z};
    b = {3_z, 2_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 7_z);
    d2 = GaloisFieldDict::from_vec(b, 7_z);
    d1.gf_div(d2, outArg(d3), outArg(d4));
    mp = d3.get_dict();
    REQUIRE(mp[0] == 6);
    REQUIRE(mp[1] == 0);
    REQUIRE(mp[2] == 1);
    REQUIRE(mp[3] == 5);
    mp = d4.get_dict();
    REQUIRE(mp[0] == 3);
    REQUIRE(mp[1] == 3);
    REQUIRE(d3 == d1/d2);

    a = {1_z};
    b = {3_z, 2_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 7_z);
    d2 = GaloisFieldDict::from_vec(b, 7_z);
    d1.gf_div(d2, outArg(d3), outArg(d4));
    REQUIRE(d3.get_dict().empty());
    mp = d4.get_dict();
    REQUIRE(mp[0] == 1);
    REQUIRE(d3 == d1/d2);

    a = {};
    d1 = GaloisFieldDict::from_vec(a, 7_z);
    d2 = d1.gf_lshift(5_z);
    REQUIRE(d2.get_dict().empty());
    d1.gf_rshift(5_z, outArg(d2), outArg(d3));
    REQUIRE(d2.get_dict().empty());
    REQUIRE(d2.get_dict().empty());
    a = {5_z, 4_z, 3_z, 2_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 7_z);
    mp = d1.gf_lshift(1_z).get_dict();
    REQUIRE(mp[0] == 0);
    REQUIRE(mp[1] == 5);
    REQUIRE(mp[2] == 4);
    REQUIRE(mp[3] == 3);
    REQUIRE(mp[4] == 2);
    REQUIRE(mp[5] == 1);
    mp = d1.gf_lshift(2_z).get_dict();
    REQUIRE(mp[0] == 0);
    REQUIRE(mp[1] == 0);
    REQUIRE(mp[2] == 5);
    REQUIRE(mp[3] == 4);
    REQUIRE(mp[4] == 3);
    REQUIRE(mp[5] == 2);
    REQUIRE(mp[6] == 1);
    d1.gf_rshift(0_z, outArg(d2), outArg(d3));
    REQUIRE(d1 == d2);
    REQUIRE(d3.get_dict().empty());
    d1.gf_rshift(5_z, outArg(d2), outArg(d3));
    mp = d2.get_dict();
    d1.gf_rshift(1_z, outArg(d2), outArg(d3));
    mp = d2.get_dict();
    REQUIRE(mp[0] == 4);
    REQUIRE(mp[1] == 3);
    REQUIRE(mp[2] == 2);
    REQUIRE(mp[3] == 1);
    REQUIRE(mp[4] == 0);
    mp = d3.get_dict();
    REQUIRE(mp[0] == 5);
    REQUIRE(mp[1] == 0);
    d1.gf_rshift(3_z, outArg(d2), outArg(d3));
    mp = d2.get_dict();
    REQUIRE(mp[0] == 2);
    REQUIRE(mp[1] == 1);
    mp = d3.get_dict();
    REQUIRE(mp[0] == 5);
    REQUIRE(mp[1] == 4);
    REQUIRE(mp[2] == 3);

    a = {8_z, 1_z, 0_z, 0_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d2 = d1.gf_pow(0_z);
    mp = d2.get_dict();
    REQUIRE(mp[0] == 1);
    REQUIRE(mp.size() == 1);
    d2 = d1.gf_pow(1_z);
    REQUIRE(d2 == d1);
    d2 = d1.gf_pow(2_z);
    mp = d2.get_dict();
    REQUIRE(mp[0] == 9);
    REQUIRE(mp[1] == 5);
    REQUIRE(mp[2] == 1);
    REQUIRE(mp[4] == 5);
    REQUIRE(mp[5] == 2);
    REQUIRE(mp[8] == 1);
    d2 = d1.gf_pow(5_z);
    mp = d2.get_dict();
    REQUIRE(mp[0] == 10);
    REQUIRE(mp[1] == 9);
    REQUIRE(mp[2] == 5);
    REQUIRE(mp[3] == 2);
    REQUIRE(mp[4] == 5);
    REQUIRE(mp[6] == 6);
    REQUIRE(mp[7] == 6);
    REQUIRE(mp[8] == 10);
    REQUIRE(mp[9] == 6);
    REQUIRE(mp[10] == 9);
    REQUIRE(mp[11] == 10);
    REQUIRE(mp[12] == 2);
    REQUIRE(mp[13] == 6);
    REQUIRE(mp[14] == 10);
    REQUIRE(mp[16] == 7);
    REQUIRE(mp[17] == 5);
    REQUIRE(mp[20] == 1);
    d2 = d1.gf_pow(8_z);
    d3 = d1.gf_pow(4_z);

    integer_class LC;
    a = {};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d1.gf_monic(LC, outArg(d2));
    REQUIRE(LC == 0_z);
    REQUIRE(d2 == d1);
    a = {1_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d1.gf_monic(LC, outArg(d2));
    REQUIRE(LC == 1_z);
    REQUIRE(d2 == d1);
    a = {2_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d1.gf_monic(LC, outArg(d2));
    REQUIRE(LC == 2_z);
    mp = d2.get_dict();
    REQUIRE(mp[0] == 1_z);
    a = {4_z, 3_z, 2_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d1.gf_monic(LC, outArg(d2));
    REQUIRE(LC == 1_z);
    REQUIRE(d2 == d1);
    a = {5_z, 4_z, 3_z, 2_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d1.gf_monic(LC, outArg(d2));
    REQUIRE(LC == 2_z);
    mp = d2.get_dict();
    REQUIRE(mp[0] == 8);
    REQUIRE(mp[1] == 2);
    REQUIRE(mp[2] == 7);
    REQUIRE(mp[3] == 1);

    a = {7_z, 8_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    a = {3_z, 2_z};
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    d1.gf_div(d2, outArg(d3), outArg(d4));
    REQUIRE(d3 == d1/d2);

    a = {};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    REQUIRE(d1.gf_gcd(d2).get_dict().empty());
    a = {2_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    mp = d2.gf_gcd(d1).get_dict();
    REQUIRE(mp[0] == 1);
    REQUIRE(d1.gf_gcd(d2).get_dict() == d2.gf_gcd(d1).get_dict());
    a = {0_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d3 = d1.gf_gcd(d2);
    REQUIRE(d1.get_dict() == d3.get_dict());
    REQUIRE(d3.get_dict() == d2.gf_gcd(d1).get_dict());

    a = {0_z, 3_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    REQUIRE(d1.gf_gcd(d2).get_dict() == d2.gf_gcd(d1).get_dict());
    mp = d1.gf_gcd(d2).get_dict();
    REQUIRE(mp[1] == 1);

    a = {7_z, 8_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    a = {7_z, 1_z, 7_z, 1_z};
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    mp = d2.gf_gcd(d1).get_dict();
    REQUIRE(mp[0] == 7);
    REQUIRE(mp[1] == 1);
    mp = d1.gf_gcd(d2).get_dict();
    REQUIRE(mp[0] == 7);
    REQUIRE(mp[1] == 1);

    a = {};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    REQUIRE(d1.gf_lcm(d2).get_dict().empty());
    a = {2_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    REQUIRE(d1.gf_lcm(d2).get_dict().empty());
    REQUIRE(d2.gf_lcm(d1).get_dict().empty());
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    mp = d1.gf_gcd(d2).get_dict();
    REQUIRE(mp[0] == 1);
    a = {0_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    a = {};
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    d3 = d1.gf_lcm(d2);
    REQUIRE(d3.get_dict().empty());
    REQUIRE(d3.get_dict() == d2.gf_lcm(d1).get_dict());
    a = {0_z, 3_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    REQUIRE(d1.gf_lcm(d2).get_dict() == d2.gf_lcm(d1).get_dict());
    mp = d1.gf_lcm(d2).get_dict();
    REQUIRE(mp[1] == 1);
    a = {7_z, 8_z, 1_z};
    d1 = GaloisFieldDict::from_vec(a, 11_z);
    a = {7_z, 1_z, 7_z, 1_z};
    d2 = GaloisFieldDict::from_vec(a, 11_z);
    REQUIRE(d1.gf_lcm(d2).get_dict() == d2.gf_lcm(d1).get_dict());
    mp = d1.gf_lcm(d2).get_dict();
    REQUIRE(mp[0] == 7);
    REQUIRE(mp[1] == 8);
    REQUIRE(mp[2] == 8);
    REQUIRE(mp[3] == 8);
    REQUIRE(mp[4] == 1);
}
