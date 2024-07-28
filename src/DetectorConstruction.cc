#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"

#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
 
  G4NistManager* man = G4NistManager::Instance();
  
  G4int ncomponents;
  G4bool isotopes = false;
  G4double fractionmass;
  
  G4Element* C = new G4Element("Carbon", "C", 6, 12.011*g/mole);
  G4Element* Cr = new G4Element("Chromium","Cr", 24., 52.00*g/mole);
  G4Element* Fe = new G4Element("Ferrum","Fe", 26., 55.85*g/mole);
  G4Element* Ni = new G4Element("Nickel","Ni", 28., 58.70*g/mole);
  G4Element* Ti = new G4Element("Titanium", "Ti", 22., 47.88*g/mole);
  
  G4Element* Ox = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
  G4Element* Ca = new G4Element("Calcium", "Ca", 20., 40.08*g/mole);
  
  auto TargetMater = 
  new G4Material("TargetMater", 0.374758 * g / cm3, ncomponents=2, kStateSolid, 293.*kelvin, 1.*atmosphere);
  
  TargetMater->AddElement(Ox, fractionmass=0.03);
  TargetMater->AddElement(Ca, fractionmass=0.97);
  
  // build materials
  //

  auto steel_mat = 
  new G4Material("steel_mat", 7.9 * g / cm3, ncomponents=5, kStateSolid, 293.*kelvin, 1.*atmosphere);
  
    steel_mat->AddElement(C, fractionmass=0.008);
    steel_mat->AddElement(Cr, fractionmass=0.18);
  
    steel_mat->AddElement(Fe, fractionmass=0.707);
    steel_mat->AddElement(Ni, fractionmass=0.10);
    steel_mat->AddElement(Ti, fractionmass=0.005); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  G4NistManager* man = G4NistManager::Instance();
  
  G4Material *world_mat = man->FindOrBuildMaterial("G4_Galactic");
  
  G4Box*
  sBox = new G4Box("World",                         //its name
                   3*m ,3*m , 3*m);   //its dimensions

  G4LogicalVolume *fLBox = new G4LogicalVolume(sBox,                     //its shape
                             world_mat,                 //its material
                             "World");                  //its name

  G4VPhysicalVolume* physWorld = new G4PVPlacement(0,                          //no rotation
                            G4ThreeVector(),            //at (0,0,0)
                            fLBox,                      //its logical volume
                            "World",                    //its name
                            0,                          //its mother  volume
                            false,                      //no boolean operation
                            0);                         //copy number
 
  G4double TargetLength      = 750.0*mm;
  G4double TargetRadius0     = 75.5*mm;
  G4double TargetRadius1     = 77.5*mm;
  G4double TargetRadius2     = 123.0*mm;
  G4double DetectorLength    = 750.0*mm; 
  G4double DetectorThickness = 2.0*mm;
  G4double WorldRadius;
  WorldRadius = TargetRadius2 + DetectorThickness;   
      
  G4Tubs* 
  empty = new G4Tubs("Empty",                                   //name
                       0., TargetRadius0, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicEmpty = new G4LogicalVolume(empty,           //shape
                             world_mat,              //material
                             "Empty");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicEmpty,                //logical volume
                           "Empty",                    //name
                           fLBox,                 //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number 
                               
   G4Material* TargetMater = man->FindOrBuildMaterial("TargetMater");
   
  G4Tubs* 
  sTarget = new G4Tubs("Target",                                   //name
                       TargetRadius1, 78.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget = new G4LogicalVolume(sTarget,           //shape
                             TargetMater,              //material
                             "Target");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget,                //logical volume
                           "Target",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
                 
  G4Tubs* 
  sTarget1 = new G4Tubs("Target1",                                   //name
                       78.5*mm, 79.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget1 = new G4LogicalVolume(sTarget1,           //shape
                             world_mat,              //material
                             "Target1");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget1,                //logical volume
                           "Target1",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
  G4Tubs* 
  sTarget2 = new G4Tubs("Target2",                                   //name
                       79.5*mm, 80.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget2 = new G4LogicalVolume(sTarget2,           //shape
                             TargetMater,              //material
                             "Target2");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget2,                //logical volume
                           "Target2",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
    G4Tubs* 
  sTarget3 = new G4Tubs("Target3",                                   //name
                       80.5*mm, 81.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget3 = new G4LogicalVolume(sTarget3,           //shape
                             world_mat,              //material
                             "Target3");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget3,                //logical volume
                           "Target3",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
                           
         G4Tubs* 
  sTarget4 = new G4Tubs("Target4",                                   //name
                       81.5*mm, 82.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget4 = new G4LogicalVolume(sTarget4,           //shape
                             TargetMater,              //material
                             "Target4");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget4,                //logical volume
                           "Target4",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number     
                           
  G4Tubs* sTarget5 = new G4Tubs("Target5",                                   //name
                                82.5*mm, 83.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget5 = new G4LogicalVolume(sTarget5,           //shape
                             world_mat,              //material
                             "Target5");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget5,                //logical volume
                           "Target5",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number  
                           
  G4Tubs* sTarget6 = new G4Tubs("Target6",                                   //name
                       83.5*mm, 84.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget6 = new G4LogicalVolume(sTarget6,           //shape
                             TargetMater,              //material
                             "Target6");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget6,                //logical volume
                           "Target6",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number  
                           
    G4Tubs* sTarget7 = new G4Tubs("Target7",                                   //name
                       84.5*mm, 85.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget7 = new G4LogicalVolume(sTarget7,           //shape
                             world_mat,              //material
                             "Target7");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget7,                //logical volume
                           "Target7",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number 
     G4Tubs* sTarget8 = new G4Tubs("Target8",                                   //name
                       85.5*mm, 86.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget8 = new G4LogicalVolume(sTarget8,           //shape
                             TargetMater,              //material
                             "Target8");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget8,                //logical volume
                           "Target8",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number 
       
    G4Tubs* sTarget9 = new G4Tubs("Target9",                                   //name
                       86.5*mm, 87.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget9 = new G4LogicalVolume(sTarget9,           //shape
                             world_mat,              //material
                             "Target9");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget9,                //logical volume
                           "Target9",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number  
                           
  G4Tubs* sTarget10 = new G4Tubs("Target10",                                   //name
                       87.5*mm, 88.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget10 = new G4LogicalVolume(sTarget10,           //shape
                             TargetMater,              //material
                             "Target10");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget10,                //logical volume
                           "Target10",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number     
   
 G4Tubs* sTarget11 = new G4Tubs("Target11",                                   //name
                       88.5*mm, 89.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget11 = new G4LogicalVolume(sTarget11,           //shape
                             world_mat,              //material
                             "Target11");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget11,                //logical volume
                           "Target11",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number 
  
   G4Tubs* sTarget12 = new G4Tubs("Target12",                                   //name
                       89.5*mm, 90.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget12 = new G4LogicalVolume(sTarget12,           //shape
                             TargetMater,              //material
                             "Target12");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget12,                //logical volume
                           "Target12",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number 
  
     G4Tubs* sTarget13 = new G4Tubs("Target13",                                   //name
                       90.5*mm, 91.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget13 = new G4LogicalVolume(sTarget13,           //shape
                             world_mat,              //material
                             "Target13");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget13,                //logical volume
                           "Target13",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
  
       G4Tubs* sTarget14 = new G4Tubs("Target14",                                   //name
                       91.5*mm, 92.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget14 = new G4LogicalVolume(sTarget14,           //shape
                             TargetMater,              //material
                             "Target14");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget14,                //logical volume
                           "Target14",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
  
         G4Tubs* sTarget15 = new G4Tubs("Target15",                                   //name
                       92.5*mm, 93.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget15 = new G4LogicalVolume(sTarget15,           //shape
                             world_mat,              //material
                             "Target15");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget15,                //logical volume
                           "Target15",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
           G4Tubs* sTarget16 = new G4Tubs("Target16",                                   //name
                       93.5*mm, 94.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget16 = new G4LogicalVolume(sTarget16,           //shape
                             TargetMater,              //material
                             "Target16");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget16,                //logical volume
                           "Target16",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
  
  G4Tubs* sTarget17 = new G4Tubs("Target17",                                   //name
                       94.5*mm, 95.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget17 = new G4LogicalVolume(sTarget17,           //shape
                             world_mat,              //material
                             "Target17");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget17,                //logical volume
                           "Target17",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
  
    G4Tubs* sTarget18 = new G4Tubs("Target18",                                   //name
                       95.5*mm, 96.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget18 = new G4LogicalVolume(sTarget18,           //shape
                             TargetMater,              //material
                             "Target18");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget18,                //logical volume
                           "Target18",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
  
      G4Tubs* sTarget19 = new G4Tubs("Target19",                                   //name
                       96.5*mm, 97.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget19 = new G4LogicalVolume(sTarget19,           //shape
                             world_mat,              //material
                             "Target19");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget19,                //logical volume
                           "Target19",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
  G4Tubs* sTarget20 = new G4Tubs("Target20",                                   //name
                       97.5*mm, 98.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget20 = new G4LogicalVolume(sTarget20,           //shape
                             TargetMater,              //material
                             "Target20");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget20,                //logical volume
                           "Target20",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
    G4Tubs* sTarget21 = new G4Tubs("Target21",                                   //name
                       98.5*mm, 99.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget21 = new G4LogicalVolume(sTarget21,           //shape
                             world_mat,              //material
                             "Target21");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget21,                //logical volume
                           "Target21",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
      G4Tubs* sTarget22 = new G4Tubs("Target22",                                   //name
                       100.5*mm, 101.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget22 = new G4LogicalVolume(sTarget22,           //shape
                             TargetMater,              //material
                             "Target22");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget22,                //logical volume
                           "Target22",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
  G4Tubs* sTarget23 = new G4Tubs("Target23",                                   //name
                       101.5*mm, 102.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget23 = new G4LogicalVolume(sTarget23,           //shape
                             world_mat,              //material
                             "Target23");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget23,                //logical volume
                           "Target23",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
 G4Tubs* sTarget24 = new G4Tubs("Target24",                                   //name
                       102.5*mm, 103.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget24 = new G4LogicalVolume(sTarget24,           //shape
                             TargetMater,              //material
                             "Target24");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget24,                //logical volume
                           "Target24",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
   G4Tubs* sTarget25 = new G4Tubs("Target25",                                   //name
                       103.5*mm, 104.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget25 = new G4LogicalVolume(sTarget25,           //shape
                             world_mat,              //material
                             "Target25");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget25,                //logical volume
                           "Target25",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
     G4Tubs* sTarget26 = new G4Tubs("Target26",                                   //name
                       104.5*mm, 105.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget26 = new G4LogicalVolume(sTarget26,           //shape
                             TargetMater,              //material
                             "Target26");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget26,                //logical volume
                           "Target26",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
       G4Tubs* sTarget27 = new G4Tubs("Target27",                                   //name
                       105.5*mm, 106.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget27 = new G4LogicalVolume(sTarget27,           //shape
                             world_mat,              //material
                             "Target27");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget27,                //logical volume
                           "Target27",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
         G4Tubs* sTarget28 = new G4Tubs("Target28",                                   //name
                       107.5*mm, 108.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions


  G4LogicalVolume *LogicTarget28 = new G4LogicalVolume(sTarget28,           //shape
                             TargetMater,              //material
                             "Target28");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget28,                //logical volume
                           "Target28",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  
  G4Tubs* sTarget29 = new G4Tubs("Target29",                                   //name
                       108.5*mm, 109.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget29 = new G4LogicalVolume(sTarget29,           //shape
                             world_mat,              //material
                             "Target29");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget29,                //logical volume
                           "Target29",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
                           
   G4Tubs* sTarget30 = new G4Tubs("Target30",                                   //name
                       109.5*mm, 110.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget30 = new G4LogicalVolume(sTarget30,           //shape
                             TargetMater,              //material
                             "Target30");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget30,                //logical volume
                           "Target30",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
                           
  G4Tubs* sTarget31 = new G4Tubs("Target31",                                   //name
                       110.5*mm, 111.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget31 = new G4LogicalVolume(sTarget31,           //shape
                             world_mat,              //material
                             "Target31");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget31,                //logical volume
                           "Target31",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb        
                           
     G4Tubs* sTarget32 = new G4Tubs("Target32",                                   //name
                       111.5*mm, 112.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget32 = new G4LogicalVolume(sTarget32,           //shape
                             TargetMater,              //material
                             "Target32");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget32,                //logical volume
                           "Target32",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb                         
  
     G4Tubs* sTarget33 = new G4Tubs("Target33",                                   //name
                       112.5*mm, 113.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget33 = new G4LogicalVolume(sTarget33,           //shape
                             world_mat,              //material
                             "Target33");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget33,                //logical volume
                           "Target33",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb   
  
  G4Tubs* sTarget34 = new G4Tubs("Target34",                                   //name
                       113.5*mm, 114.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget34 = new G4LogicalVolume(sTarget34,           //shape
                             TargetMater,              //material
                             "Target34");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget34,                //logical volume
                           "Target34",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb 
  
    G4Tubs* sTarget35 = new G4Tubs("Target35",                                   //name
                       114.5*mm, 115.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget35 = new G4LogicalVolume(sTarget35,           //shape
                             world_mat,              //material
                             "Target35");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget35,                //logical volume
                           "Target35",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb 
      G4Tubs* sTarget36 = new G4Tubs("Target36",                                   //name
                       115.5*mm, 116.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget36 = new G4LogicalVolume(sTarget36,           //shape
                             TargetMater,              //material
                             "Target36");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget36,                //logical volume
                           "Target36",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb 
        G4Tubs* sTarget37 = new G4Tubs("Target37",                                   //name
                       116.5*mm, 117.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget37 = new G4LogicalVolume(sTarget37,           //shape
                             world_mat,              //material
                             "Target37");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget37,                //logical volume
                           "Target37",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb 
  G4Tubs* sTarget38 = new G4Tubs("Target38",                                   //name
                       117.5*mm, 118.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget38 = new G4LogicalVolume(sTarget38,           //shape
                             TargetMater,              //material
                             "Target38");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget38,                //logical volume
                           "Target38",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb 
    G4Tubs* sTarget39 = new G4Tubs("Target39",                                   //name
                       118.5*mm, 119.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget39 = new G4LogicalVolume(sTarget39,           //shape
                             world_mat,              //material
                             "Target39");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget39,                //logical volume
                           "Target39",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb 
  
      G4Tubs* sTarget40 = new G4Tubs("Target40",                                   //name
                       119.5*mm, 120.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget40 = new G4LogicalVolume(sTarget40,           //shape
                             TargetMater,              //material
                             "Target40");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget40,                //logical volume
                           "Target40",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb 
  
   G4Tubs* sTarget41 = new G4Tubs("Target41",                                   //name
                       120.5*mm, 121.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget41 = new G4LogicalVolume(sTarget41,           //shape
                             world_mat,              //material
                             "Target41");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget41,                //logical volume
                           "Target41",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
  G4Tubs* sTarget42 = new G4Tubs("Target42",                                   //name
                       121.5*mm, 122.5*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget42 = new G4LogicalVolume(sTarget42,           //shape
                             TargetMater,              //material
                             "Target42");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget42,                //logical volume
                           "Target42",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
    G4Tubs* sTarget43 = new G4Tubs("Target43",                                   //name
                       122.5*mm, 123.0*mm, 0.5*TargetLength, 0.,twopi); //dimensions
  
   G4LogicalVolume *LogicTarget43 = new G4LogicalVolume(sTarget43,           //shape
                             world_mat,              //material
                             "Target43");                 //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicTarget43,                //logical volume
                           "Target43",                    //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy numb
                           
  G4Material* steel_mat = man->FindOrBuildMaterial("steel_mat"); 
  // Detector
  //
  
  G4Tubs* 
  sDetector2 = new G4Tubs("Detector2",  
                         TargetRadius0, TargetRadius1, 0.5*DetectorLength, 0.,twopi);


  G4LogicalVolume *LogicDetector2 = new G4LogicalVolume(sDetector2,       //shape
                             steel_mat,            //material
                             "Detector2");               //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicDetector2,              //logical volume
                           "Detector2",                  //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
  
  G4Tubs* 
  sDetector = new G4Tubs("Detector",  
                         TargetRadius2, WorldRadius, 0.5*DetectorLength, 0.,twopi);


  G4LogicalVolume *LogicDetector = new G4LogicalVolume(sDetector,       //shape
                              steel_mat,            //material
                             "Detector");               //name
                               
           new G4PVPlacement(0,                         //no rotation
                           G4ThreeVector(),             //at (0,0,0)
                           LogicDetector,              //logical volume
                           "Detector",                  //name
                           fLBox,                      //mother  volume
                           false,                       //no boolean operation
                           0);                          //copy number
                                                  
  //always return the root volume
  //
  return physWorld;
}

