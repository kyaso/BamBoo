#define CLOCK(clk, period) if((time%period)==0)clk=1; if((time%period)==(period/2))clk=0;
#define SET_SIG(t, sig, val) if(time == t) { sig = val; }