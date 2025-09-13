# Plant Tournament

## Introduction

This project is a two-player battle game with 5 characters. Enjoy the battle with your friends!

## Characters

1. Pea Shooter:
   1. Normal Attack: Shoots peas, cooldown 100ms, attack power 10
   2. Ultimate: Transforms into Machine Gun Pea, rapidly shoots many peas, attack power 10
2. Sunflower:
   1. Normal Attack: Launches a small sun that explodes on contact with the opponent, cooldown 300ms, attack power 20
   2. Ultimate: Generates a huge red sun that slowly descends from above the opponent, used to restrict opponent's movement, produces SUN effect above the character, attack power 40
3. Puff-shroom:
   1. Normal Attack: Shoots spores in all directions in a small range, cooldown 700ms, attack power 25
   2. Ultimate: Shoots spores in all directions in a large range, attack power 30
4. Butter Puff-shroom:
   1. Normal Attack: Shoots spores in all directions in a small range, cooldown 1500ms, attack power 15, by default shoots butter in eight directions, butter can immobilize the opponent for 3000ms
   2. Ultimate: Shoots a huge corn cannon projectile at the opponent, dealing 80 damage on hit
5. Wall-nut:
   1. Normal Attack: Teleports forward a certain distance, cooldown 50ms, no attack power
   2. Ultimate: Instantly overheats and explodes massively, dealing 50 damage to the opponent
   3. Special Note: Wall-nut slowly recovers health and increases mana during the game

## Buff System

1. Appearance Condition: Each game tick has a certain chance to generate a buff bullet
2. Bullet Types:
   1. Silence: Prevents the character from attacking, lasts 1000ms
   2. Haste: Increases character speed to 1.5 times, lasts 3000ms
   3. Regen: Slowly recovers the character's health, lasts 3000ms
   4. Invisibility: Hides the character's body, lasts 3000ms

## Configuration File

- You can modify many variable configurations in the config file, such as:
  1. Player's maximum health
  2. Attack power of each character's bullets
  3. Duration of each effect
  4. Multipliers for regen and haste
  5. Probability of buff bullets appearing **Higher values mean lower probability**
  6. Player's key configurations
  7. Character's attack cooldown times
  8. Energy gained after hitting with attacks
  9. Pea flight speed
  10. Sun launch speed
  11. Gravity constant **Not recommended to modify**
  12. Character size **Not recommended to modify**
  13. Character running speed
  14. Character jumping speed
  15. **Platform positions, sizes, styles**

## Build and Run

1. Preparation: Install Visual Studio and EasyX Summer Edition, download the current code
2. Open the sln file in the project with Visual Studio
3. Configure vcpkg and use it to install nlohmann json.
4. Select configuration, **compile** and **run**!

## Dependencies

1. nlohmann json
2. EasyX

## AI Usage

Almost no AI used, some code used AI autocomplete, never used AI chat functions to ask about code issues, README translation and git commit message generated using AI
