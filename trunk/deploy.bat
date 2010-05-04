del dist\*.exe
setup.py build_ext
setup.py build_py -c -O2 -f
setup.py bdist_wininst
copy dist\*.exe \\winfile\fb09-ilr\Alle\Philipp\cmf_dist\binary
