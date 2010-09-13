del dist\*.exe
setup.py build_ext swig
setup.py build_py -c -O2 -f
setup.py bdist_wininst
setup.py sdist --formats=zip
setup.py sdist --formats=gztar
copy dist\*.exe \\winfile\fb09-ilr\Alle\Philipp\cmf_dist\binary
copy dist\*.zip \\winfile\fb09-ilr\Alle\Philipp\cmf_dist\


@echo Copy doxy-doc
xcopy doxy\html \\winfile\fb09-ilr\Alle\Philipp\cmf_dist\doc\html\ /s /m /q /y