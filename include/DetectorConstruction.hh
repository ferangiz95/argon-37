#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"


class G4LogicalVolume;
class G4Material;
class G4VPhysicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction() override;
  
  public:
    virtual G4VPhysicalVolume *Construct() override;
    
  private:   
     void DefineMaterials();
     G4bool fCheckOverlaps = true; 
     G4VPhysicalVolume* ConstructVolumes();    
};

#endif

