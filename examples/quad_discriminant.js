function QUAD(A,B,C) { 
    var N;
    N = B*B-4*A*C;
    if(N<0) {
        return 0;
    }
    else {
        if(N=0) {
            return 1;
        }
    else {
        return 2;
      }
   }
    return 0;
} 

function MAIN() { 
    var D;
    D = QUAD(1,0-6,9);
    if(D=0) {
        console.log('No Roots');
    }
    else {
      if(D=1) {
        console.log('Discriminant is zero');
      }
      else {
        console.log('Two roots are available');
      }
   }

   return true;
} 
