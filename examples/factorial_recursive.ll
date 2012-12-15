; ModuleID = 'main_module'

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00"

define double @fact(double %D) {
func:
  %0 = alloca double
  %D1 = alloca double
  store double %D, double* %D1
  %1 = load double* %D1
  %var = fcmp ule double %1, 0.000000e+00
  %ifcond = icmp eq i1 %var, true
  br i1 %ifcond, label %then, label %else

exit:                                             ; preds = %merge, %else, %then
  %2 = load double* %0
  ret double %2

then:                                             ; preds = %func
  store double 1.000000e+00, double* %0
  br label %exit

else:                                             ; preds = %func
  %3 = load double* %D1
  %4 = load double* %D1
  %var2 = fsub double %4, 1.000000e+00
  %callfunc = call double @fact(double %var2)
  %var3 = fmul double %3, %callfunc
  store double %var3, double* %0
  br label %exit

merge:                                            ; No predecessors!
  store double 1.000000e+00, double* %0
  br label %exit
}

define i1 @main() {
func:
  %0 = alloca i1
  %D = alloca double
  store double 0.000000e+00, double* %D
  br label %loop

exit:                                             ; preds = %exit1
  %1 = load i1* %0
  ret i1 %1

loop:                                             ; preds = %body, %func
  %2 = load double* %D
  %var = fcmp ule double %2, 1.000000e+01
  %condition = icmp eq i1 %var, true
  br i1 %condition, label %body, label %exit1

body:                                             ; preds = %loop
  %3 = load double* %D
  %callfunc = call double @fact(double %3)
  %4 = call i32 (i8*, double, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @0, i32 0, i32 0), double %callfunc)
  %5 = load double* %D
  %var2 = fadd double %5, 1.000000e+00
  store double %var2, double* %D
  br label %loop

exit1:                                            ; preds = %loop
  store i1 true, i1* %0
  br label %exit
}

declare i32 @printf(i8*, double, ...)
