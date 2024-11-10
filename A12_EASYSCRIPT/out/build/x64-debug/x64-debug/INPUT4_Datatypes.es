# EasyScript Example 4
The program is lexically correct about the SOFIA city 
#
main&(int FoundationYear$;
	  real Area%, HDI%;
	  string CountryName@;) {
	data {
	}
	code {
		FoundationYear$ = -7000;
		Area% = 492.0;
		HDI%=0.871;
		CountryName@ = 'Bulgary';
		print&(CountryName$);
		return();
	}
}
