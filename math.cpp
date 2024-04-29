#include "Memory.hpp"
#include "includes.h"
#include <iostream>

namespace Offset
{
    // client
    constexpr std::uintptr_t baseAddr = 0x00400000;
    constexpr std::uintptr_t LocalEntity = 0x17E0A8;
    constexpr std::uintptr_t entityList = 0x18AC04;
    const int posX = 0x2C;
    const int posY = 0x28;
    const int posZ = 0x30;
}

double distance(double x1, double y1, double z1, double x2, double y2, double z2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}

int main()
{
    const auto memory = Memory{L"ac_client.exe"};

    const auto client = memory.GetModuleAddress(L"ac_client.exe");

    // ローカルプレイヤーの座標を取得
    uintptr_t LocalPlayer = memory.Read<uintptr_t>(client + Offset::LocalEntity);
    if (LocalPlayer == 0)
    {
        std::cerr << "Failed to read local player address." << std::endl;
        return 1;
    }

    double LocalPlayerPosX = memory.Read<double>(LocalPlayer + Offset::posX);
    double LocalPlayerPosY = memory.Read<double>(LocalPlayer + Offset::posY);
    double LocalPlayerPosZ = memory.Read<double>(LocalPlayer + Offset::posZ);

    for (int i = 0; i < 10; i++)
    {
        uintptr_t EntityList = memory.Read<uintptr_t>(client + Offset::entityList);
        if (EntityList == 0)
        {
            std::cerr << "Failed to read entity list address." << std::endl;
            return 1;
        }

        uintptr_t EntityPointer = memory.Read<uintptr_t>(EntityList + i * 4);
        if (EntityPointer == 0)
            continue;

        double EntityPosX = memory.Read<double>(EntityPointer + Offset::posX);
        double EntityPosY = memory.Read<double>(EntityPointer + Offset::posY);
        double EntityPosZ = memory.Read<double>(EntityPointer + Offset::posZ);

        double dist = distance(LocalPlayerPosX, LocalPlayerPosY, LocalPlayerPosZ, EntityPosX, EntityPosY, EntityPosZ);

        // 近くにいるエンティティとその距離を出力
        std::cout << "Entity " << i << ": Distance = " << dist << std::endl;
    }

    return 0;
}
