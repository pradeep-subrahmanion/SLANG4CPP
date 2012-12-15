; ModuleID = 'main_module'

@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00"

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
  %var = fcmp ule double %2, 1.000000e+02
  %condition = icmp eq i1 %var, true
  br i1 %condition, label %body, label %exit1

body:                                             ; preds = %loop
  %3 = load double* %D
  %4 = call i32 (i8*, double, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @0, i32 0, i32 0), double %3)
  %5 = load double* %D
  %var2 = fadd double %5, 1.000000e+00
  store double %var2, double* %D
  br label %loop

exit1:                                            ; preds = %loop
  store i1 true, i1* %0
  br label %exit
}

declare i32 @printf(i8*, double, ...)
