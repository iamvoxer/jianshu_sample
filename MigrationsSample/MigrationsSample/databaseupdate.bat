set EfMigrationsNamespace=%1
set EfMigrationsDllName=%1.dll
set EfMigrationsDllDepsJson=%1.deps.json
set DllDir=%cd%
set PathToNuGetPackages=%USERPROFILE%\.nuget\packages
set PathToEfDll=%PathToNuGetPackages%\microsoft.entityframeworkcore.tools\2.0.0\tools\netcoreapp2.0\ef.dll

dotnet exec --depsfile .\%EfMigrationsDllDepsJson% --additionalprobingpath %PathToNuGetPackages% %PathToEfDll% database update --assembly .\%EfMigrationsDllName% --startup-assembly .\%EfMigrationsDllName% --data-dir %DllDir% --verbose --root-namespace %EfMigrationsNamespace%  
