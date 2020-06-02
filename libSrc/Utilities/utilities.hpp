// utilities.hpp
// Adam C. Burke (adam.burke603@gmail.com)
// June 2, 2020

#pragma once

namespace MPS{

    class ScopeTimer {
        private:
        std::__1::chrono::steady_clock::time_point start;
        std::__1::chrono::steady_clock::time_point end;
        public:
            ScopeTimer();
            ~ScopeTimer();
        };
}