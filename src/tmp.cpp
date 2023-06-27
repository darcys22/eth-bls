#include "eth-bls/tmp.hpp"

int tmp::add(int a, int b) { return a + b; }

int64_t tmp::request() { 
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                      cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                      cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    return r.status_code;                  // 200
    //r.header["content-type"];       // application/json; charset=utf-8
    //r.text;                         // JSON text string
}
