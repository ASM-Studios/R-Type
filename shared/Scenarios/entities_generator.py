import os
import random
import sys
import time

def generate_entities_cfg(file_name, num_entities):
    behaviors = ["classic", "sinusoidal", "zig-zag"]
    sprite_ids = [14, 15, 16, 58, 59]
    entities = []
    delay_time = 1.0
    random.seed(time.time())
    round_size = random.randint(4, 8)
    n = 0

    for i in range(num_entities):
        if n > round_size:
            n = 0
            delay_time += 5
            round_size += random.randint(1, 5)
        entity = {
            "name": "enemy",
            "id": i + 11,
            "delay_time": round(delay_time, 2),
            "components": [
                {"type": "Position"},
                {"type": "Sprite", "spriteID": sprite_ids[i % len(sprite_ids)], "stateID": 0},
                {"type": "Behavior", "model": behaviors[i % len(behaviors)]}
            ]
        }
        n += 1
        entities.append(entity)
        delay_time += 1.0

    with open(file_name, 'w') as file:
        file.write("entities = (\n")
        for entity in entities:
            file.write("    {\n")
            file.write(f"        name = \"{entity['name']}\";\n")
            file.write(f"        id = {entity['id']};\n")
            file.write(f"        delay_time = {entity['delay_time']};\n")
            file.write("        components = (\n")
            for component in entity['components']:
                file.write("            {\n")
                file.write(f"                type = \"{component['type']}\";\n")
                if component['type'] == 'Sprite':
                    file.write(f"                spriteID = {component['spriteID']};\n")
                    file.write(f"                stateID = {component['stateID']};\n")
                elif component['type'] == 'Behavior':
                    file.write(f"                model = \"{component['model']}\";\n")
                file.write("            },\n")
            file.write("        );\n")
            file.write("    },\n")
        file.write(");\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 entities_generator.py <num_entities>")
        sys.exit(1)

    try:
        num_entities = int(sys.argv[1])
    except ValueError:
        print("Error: The number of entities must be an integer.")
        sys.exit(1)
    output_file = f"output{num_entities}.cfg"
    generate_entities_cfg(output_file, num_entities)
    print(f"Entities file generated: {output_file}")
