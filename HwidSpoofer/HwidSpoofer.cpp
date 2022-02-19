#include <iostream>
#include <Windows.h>
#include "Tchar.h"
#include <vector>
#include <typeinfo>
#include <d3d9.h>
#include <string>

std::vector<std::string> getListOfDrives() {
    std::vector<std::string> arrayOfDrives;
    char* szDrives = new char[MAX_PATH]();
    if (GetLogicalDriveStringsA(MAX_PATH, szDrives));
    for (int i = 0; i < 100; i += 4)
        if (szDrives[i] != (char)0)
            arrayOfDrives.push_back(std::string{ szDrives[i],szDrives[i + 1],szDrives[i + 2] });
    delete[] szDrives;
    return arrayOfDrives;
}


std::string HexStringGenerator(byte length) {
    char hex_characters[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
    std::string Hex_String;
    for (int i = 0; i < length; i++)
    {
        Hex_String += hex_characters[rand() % 16];
    }
    return Hex_String;
}


std::string HddSerialGenerator() {
    return HexStringGenerator(4) + "-" + HexStringGenerator(4);
}

std::string GuidSerialGenerator() {
    return HexStringGenerator(8) + "-" + HexStringGenerator(4) + "-" + HexStringGenerator(4) + "-" + HexStringGenerator(4) + HexStringGenerator(12);
}

std::string dec2hex(int n, std::string x) {

    std::string hexa = { "0123456789ABCDEF" };
    return (n < 16 ? "" + (hexa[n] + x) : dec2hex(n / 16, hexa[n % 16] + x));

}


int main()
{
    /*
    * TODO: GPU ID Spoofing 
    IDirect3D9* d3dobject = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpresent;
    memset(&d3dpresent, 0, sizeof(D3DPRESENT_PARAMETERS));
    d3dpresent.Windowed = TRUE;
    d3dpresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
    UINT adaptercount = d3dobject->GetAdapterCount();
    D3DADAPTER_IDENTIFIER9* adapters = (D3DADAPTER_IDENTIFIER9*)malloc(sizeof(D3DADAPTER_IDENTIFIER9) * adaptercount);

    for (int i = 0; i < adaptercount; i++)
    {
        d3dobject->GetAdapterIdentifier(i, 0, &(adapters[i]));
    }

    // Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\PCI
    // VEN_10DE&DEV_1C03&SUBSYS_00000000&REV_A1

    const int vendor_id = adapters->VendorId;
    const int device_id = adapters->DeviceId;
    const int subsys_id = adapters->SubSysId;

    std::string str_vendor_id;
    std::string str_device_id;
    std::string str_subsys_id;


    std::string gpu = "VEN_" + dec2hex(vendor_id, str_vendor_id) + "&DEV_" + dec2hex(device_id, str_device_id) + "&SUBSYS_" + dec2hex(subsys_id, str_subsys_id);

    std::cout << gpu << std::endl;

    std::cout << std::hex << adapters->VendorId << std::endl;
    std::cout << std::hex << adapters->DeviceId << std::endl;
    std::cout << std::hex << adapters->SubSysId << std::endl;
    std::cout << std::hex << adapters->Revision << std::endl;
    */

    srand(time(0));

    std::vector<std::string> drives = getListOfDrives();
    
    std::cout << "Changing Disk serials" << std::endl;

    for (std::string currentDrive : drives) {
        currentDrive.erase(remove(currentDrive.begin(), currentDrive.end(), '\\'), currentDrive.end());
        system(("Volumeid64.exe " + currentDrive + " " + HddSerialGenerator()).c_str());
    }

    std::cout << "Disk serials changed" << std::endl << std::endl;

    std::cout << "Changing guilds" << std::endl << std::endl;
    
    std::string guidValue = GuidSerialGenerator();


    std::cout << "Changing MachineGuid";

    // set cryptography machineguid
    system(("REG ADD " + std::string("\"") + std::string("HKLM\\SOFTWARE\\Microsoft\\Cryptography") + std::string("\" ") + "/v MachineGuid /t REG_SZ /d " + std::string("\"" + guidValue + "\"") + " /f").c_str());

    std::cout << "\nChanging HwProfileGuid";

    // set hardware guid
    system(("REG ADD " + std::string("\"") + std::string("HKLM\\SYSTEM\\CurrentControlSet\\Control\\IDconfigDB\\Hardware Profiles\\0001") + std::string("\" ") + "/v HwProfileGuid /t REG_SZ /d " + std::string("\"{" + guidValue + "\"}") + " /f").c_str());

    std::cout << "\nChanging MacAddr";


    // change mac address
    system(("REG ADD " + std::string("\"") + std::string("HKLM\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0001") + std::string("\" ") + "/v NetworkAddress /t REG_SZ /d " + std::string("\"" + HexStringGenerator(12) + "\"") + " /f").c_str());

    std::cout << "\nChanging motherboard id, may not work!" << std::endl;

    // change motherboard id
    system(("AMIDEWINx64.EXE /SU " + HexStringGenerator(32)).c_str());
    
    std::cout << "\n\nRestart your computer!" << std::endl;

}
