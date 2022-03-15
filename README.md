# EDM_openfoam
**Eddy dissipation combustion model updated for Openfoam 9**

Copy the whole src directory into user project folder:
```
foam
cp -r --parents src/combustionModels $WM_PROJECT_USER_DIR
cd $WM_PROJECT_USER_DIR/src/combustionModels
```
Download the code to the src directory:
```
git clone http://github.com/<yourRepoLink> <$WM_PROJECT_USER_DIR/src/combustionModels>
git archive -o output.zip
```
Add EDM/EDM.C to Make/files using Vim
```
vim Make/files
```
Compile:
```
wmake
```
