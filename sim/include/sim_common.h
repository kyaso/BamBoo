#define SIM_INIT uint64_t time = 0; int errors = 0;
#define SIM_START(duration) while(time < duration) {
#define SIM_END }
#define SIM_ADV_TIME time++;
#define CLOCK(clk, period) if((time%period)==0)clk=1; if((time%period)==(period/2))clk=0;
#define SET_SIG(t, sig, val) if(time == t) { sig = val; }

#define CHECK(t, sig, gold_val) if(time == t) { if(sig != gold_val) errors++; }
#define CHECK2(sig, gold_val) if(sig != gold_val) errors++;
#define TEST_PASSFAIL if(errors == 0) { \
        std::cout << "0 Errors" << std::endl; \
        std::cout << "<<<< TEST PASSED! >>>>" << std::endl; \
    } \
    else { \
        std::cout << errors << " Error(s)" << std::endl; \
        std::cout << "<<<< TEST FAILED >>>>" << std::endl; \
    }