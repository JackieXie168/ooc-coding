@echo OFF
REM
REM Running test cases as a test suite
REM 
REM Windows version - without memory leak
REM
REM #########################################x


@ooctest
@exceptiontest
REM @testcasetest
@refcountedtest
@vectortest
@listtest
@signaltest
