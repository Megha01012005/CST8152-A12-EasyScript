# EasyScript Example 2:
  Volume of a sphere #
main&(real r%;) {
	data {
		real PI%, Vol%;
	}
	code {
		PI% = 3.14;
		Vol% = 4.0 / 3.0 * PI% * (r% * r% * r%);
		print&(Vol%);
	}
}
