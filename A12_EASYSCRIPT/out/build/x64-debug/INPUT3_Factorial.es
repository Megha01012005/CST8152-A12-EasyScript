# EasyScript Example 3:
  Testing numbers (ex: factorial) #
main&() {
    data {
        int i$, n$, f$;
    }
    code {
        f$=1;
        i$=1;
        print&('Write a number:');
        input&(n$);
        while (i$<n$) do {
            f$ = f$ * i$;
            i$ = i$ + 1;
        };
        print&('Factorial:');
        print&(f$);
		return(f$);
    }
}
