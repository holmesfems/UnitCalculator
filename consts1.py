#setUnit kg=1.0 mass
#setUnit m=1.0 length
#setUnit sec=1.0 time
#setUnit K=1.0 temperature
#setUnit A=1.0 current
#setUnit mol=1.0 substance_quantity
#setUnit cd=1.0 luminosity
%J=%kg*%m^2/%sec^2
%C=%A*%sec
%N=%kg*%m/%sec^2
%V=%J/%C

math.pi=3.1415926535897

time.minute=60.0*%sec
time.hour=60.0*time.minute
time.day=24.0*time.hour
time.year=365.25*time.day

astro.G=6.67408*10^-11*%m^3/%kg/%sec^2
astro.c0=299792458.0*%m/%sec
astro.cyear=astro.c0*time.year
astro.pc=3.085677581*10^16*%m
astro.au=149597870700.0*%m
astro.mSun=1.9884*10^30%kg
astro.rSun=695700000.0*%m
astro.mJup=1.8986*10^27*%kg
astro.orJup=778412010000.0*%m
astro.opJup=11.86155*time.year

atom.u=1.660539040*10^-27%kg
atom.mH=1.007825*atom.u

quantum.k=1.38064852*10^-23*%J/%K
quantum.h=6.626070040*10^-34*%J*%sec
quantum.e=1.6021766208*10^-19*%C
%eV=quantum.e*%V
quantum.hbar=quantum.h/(2*math.pi)
quantum.mMu=105.6583666838*1*10^6*%eV/astro.c0^2
quantum.mE=9.1093835611*10^-31*%kg
