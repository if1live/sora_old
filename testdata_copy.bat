mkdir Debug/testdata
robocopy testdata Debug/testdata /E

mkdir Release/testres
robocopy testdata Release/testdata /E

mkdir Debug/resource
robocopy resource Debug/resource /E

mkdir Release/resource
robocopy resource Release/resource /E