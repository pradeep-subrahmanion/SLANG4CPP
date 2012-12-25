function FACT(D) { 
if(D<=0) {
return 1;
 }
else {
return D*FACT(D-1);
 }
return 1;
} 
function MAIN() { 
var D;
D = 0;
while(D<=10) {
print(FACT(D)
);
D = D+1;
 }
return true;
} 
MAIN()