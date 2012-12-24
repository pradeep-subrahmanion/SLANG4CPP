function FIB(N) { 
    if(N<=1) {
        return 1;
    }
    else {
        return FIB(N-1)+FIB(N-2);
    }
} 

function MAIN() { 
    var D;
    D = 0;
    while(D<=10) {
        console.log(FIB(D));
        D = D+1;
    }
} 
