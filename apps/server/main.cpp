//
// Created by hubert on 26.12.2020.
//
#include <spdlog/spdlog.h>
#include <tmp.h>

int main(){
    tmp a;
    spdlog::info("server hw! {}", a.t(5));
}
