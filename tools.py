import os
import time
import random
from curses.ascii import isdigit
import operator
import characters as ch

matnames = ['WOOD', 'STONE', 'IRON', 'SILVER', 'PLATINUM']
quanames = ['NO', 'WOOD', 'STONE', 'IRON', 'SILVER', 'PLATINUM']

equipnames = ['Helmet', 'Chestplate', 'Trousers', 'Boots', 'Sword', 'Arrow']
cost = [10, 15, 12, 8, 16, 1]
attributes = [8, 14, 10, 5, 40, 40]

maxrounds = 100

def rn(x, y=0): # get a random integer from 0 to x or from x to y
    if y == 0:
        return random.randint(0, x)
    return random.randint(x, y)
    
def rl(x, room): # get a random number which tends to be bigger when the player walks through more rooms.
    return random.randint(0, int(x * room))

def rnl(x, y, room): # combine the above 2 functions
    return random.randint(0, x) + random.randint(0, int(y * room))

def pause(info = 'continue'): # pause the game, tells the player to press ENTER
    print('Press ENTER to ' + info + '.', end = '\r')
    input()

def clear(): # clear the screen
    os.system('clear')

def wait(seconds): # wait a few seconds
    time.sleep(seconds)

def printheader(s = '', mid = False): # print a header
    s += ' '
    if mid:
        print(f'{s:━^120}')
        return
    print(f'{s:━<120}')

def getchoice_without_printing(choices, getmove = False):
    # choices: a tuple of possible choices
    # return: the index of choice (starting with 0)
    if getmove:
        moves = {'W': '3', 'w': '3', 'A': '1', 'a': '1', 'S': '2', 's': '2', 'D': '4', 'd': '4'}
        #print('You can use WASD or the numbers below to move.')
    #for i in range(len(choices)):
    #    print(i + 1, ': ', choices[i], sep = '', end = '    ')
    #print()
    choice = 0
    while True:
        choice = input()
        if getmove and choice in moves:
            choice = moves[choice]
        if not choice.isdigit():
            print('Invalid input')
            continue
        choice = int(choice)
        if choice < 1 or choice > len(choices):
            print('Invalid input')
            continue
        return choice - 1

def getchoice(choices, getmove = False): # get user input with a few choices, detects invalid input
    # choices: a tuple/list of possible choices
    # return: the index of choice (starting with 0)
    if getmove:
        moves = {'W': '3', 'w': '3', 'A': '1', 'a': '1', 'S': '2', 's': '2', 'D': '4', 'd': '4'}
        #print('You can use WASD or the numbers below to move.')
    for i in range(len(choices)):
        print(i + 1, ': ', choices[i], sep = '', end = '    ')
    print()
    choice = 0
    while True:
        choice = input()
        if getmove and choice in moves:
            choice = moves[choice]
        if not choice.isdigit():
            print('Invalid input')
            continue
        choice = int(choice)
        if choice < 1 or choice > len(choices):
            print('Invalid input')
            continue
        return choice - 1

def printt(data="",second=1): # prints a message and waits
    print(data)
    wait(second)

def getcolor(color): # get the color code used in console from a string of color
    if color == 'red':
        return '\33[91m'
    if color == 'green':
        return '\33[92m'
    if color == 'darkgreen':
        return '\33[32m'
    if color == 'purple':
        return '\33[35m'
    if color == 'brown':
        return '\33[33m'
    if color == 'blue':
        return '\33[34m'
    if color == 'gray':
        return '\33[90m'
    if color == 'lightpurple':
        return '\33[95m'
    return '\33[0m'

def printfile(filename, waittime = 3): # print all the text from a file
    with open(filename) as file:
        for line in file:
            print(line, end = '')
            wait(waittime)

def workshop(player): # workshop
    print('You enter a workshop.')
    pause('start crafting')
    clear()
    while True:
        printheader('Status')
        player.printstatus()
        printheader('Workshop')
        print('Your equipment:')
        for i in range(5):
            print(quanames[player.equipment[i]], equipnames[i], '(Require', cost[i], 'selected material)')
        print('Bow')
        player.printquiver()
        printheader()
        print('Choose what you want to craft')
        equip = getchoice(equipnames + ['Switch arrow', 'Exit'])
        if equip == 6:
            player.switcharrow()
            clear()
            continue
        if equip == 7:
            break
        print('Choose the quality of', equipnames[equip])
        mat = getchoice(matnames + ['Go back'])
        if mat == 5:
            clear()
            continue
        qua = mat + 1
        if 0 <= equip <= 4:
            if player.materials[mat] >= cost[equip]:
                print('Crafting. Please wait ...')
                wait(rn(2) + 1)
                player.materials[mat] -= cost[equip]
                oldmat = player.equipment[equip] - 1
                if equip == 4:
                    if oldmat != -1:
                        player.attack -= int(attributes[4] * 1.5 ** oldmat)
                    player.attack += int(attributes[4] * 1.5 ** mat)
                else:
                    if oldmat != -1:
                        player.defense -= int(attributes[equip] * 1.5 ** oldmat)
                    player.defense += int(attributes[equip] * 1.5 ** mat)
                player.equipment[equip] = qua
                print(matnames[mat], equipnames[equip], 'is crafted successfully')
                pause('go back')
                clear()
            else:
                print('Not enough materials')
                pause('go back')
                clear()
        if equip == 5:
            print('How many', matnames[mat], 'arrows do you wish to craft? (1 material = 1 arrow)')
            while True:
                num = input()
                if not num.isdigit() or int(num) == 0:
                    print('Invalid input')
                    continue
                num = int(num)
                break
            if player.materials[mat] >= num:
                print('Crafting. Please wait ...')
                wait(rn(2) + 1)
                player.materials[mat] -= num
                player.arrows[mat] += num
                if player.primaryarrow is None: player.primaryarrow = mat
                print(num, matnames[mat], 'Arrow is crafted successfully.')
                pause('go back')
                clear()
            else:
                print('Not enough material.')
                pause('go back')
                clear()

def calcrangedmg(arrow): # calculates the damage of an arrow from a certain type of material
    return int(40 * (1.5 ** arrow))

def battle(player, enemies): # conducts a battle between a player and an enemy. Enemies can be created using getenemy()
    pause('start battle')
    clear()
    if type(enemies) != list:
        enemies = [enemies]
    temp_attack = player.attack
    temp_defense = player.defense
    fleechance = 0
    meleefi = None
    rangedfi = None
    rewards = [0] * 5
    autoattack = False
    for i in range(len(enemies)):
        fleechance = max(fleechance, enemies[i].fleechance)
        if meleefi == None:
            meleefi = i
        elif enemies[i].attack > enemies[meleefi].attack:
            meleefi = i
        if rangedfi == None and enemies[i].ranged == True:
            rangedfi = i
        elif enemies[i].ranged == True and enemies[i].attack > enemies[rangedfi].attack:
            rangedfi = i
        rewards = list(map(operator.add, rewards, enemies[i].materials))
    meleefi = enemies[meleefi].failinfo
    if rangedfi != None:
        rangedfi = enemies[rangedfi].failinfo
    rounds = 1
    fled = False
    while rounds <= 100:
        printheader('Battle - Round ' + str(rounds) + '/100')
        player.printstatus()
        if player.primaryarrow is not None:
            print('Primary arrow: ', matnames[player.primaryarrow], ' (', player.arrows[player.primaryarrow], ' Left)', sep = '')
        else:
            print('No arrow equipped')
        print('VERSUS')
        ch.printenemies(enemies)
        printheader('Your Turn')
        if autoattack:
            choice = 0
        else:
            if len(enemies) > 1:
                choice = getchoice(('Melee attack', 'Ranged attack', 'Switch arrow', \
                'Flee (' + str(round(fleechance * 100)) + '% chance)', 'Auto attack'))
            else:
                sworddamage = max(player.attack - enemies[0].defense, 0)
                swordterm = 'Melee attack' + ' (' + str(sworddamage) + ' Damage)'
                if player.primaryarrow is not None and player.arrows[player.primaryarrow]:
                    bowdamage = max(calcrangedmg(player.primaryarrow) - enemies[0].defense, 0)
                    bowterm = 'Ranged attack' + ' (' + str(bowdamage) + ' Damage)'
                else:
                    bowterm = 'Ranged attack (Not available)'
                fleeterm = 'Flee (' + str(round(fleechance * 100)) + '% chance)'
                choice = getchoice((swordterm, bowterm, 'Switch arrow', fleeterm, 'Auto attack'))
        if choice == 2:
            player.switcharrow()
            clear()
            continue
        if choice == 4:
            choice = 0
            autoattack = True
        if choice == 0 or choice == 1:
            if choice == 1 and player.primaryarrow is None:
                print('You have not equipped an arrow.')
                pause('go back')
                clear()
                continue
            if choice == 1 and player.arrows[player.primaryarrow] == 0:
                print('You do not own the selected type of arrow.')
                pause('go back')
                clear()
                continue
            if autoattack or len(enemies) == 1:
                enemy = 0
            else:
                print('Select an enemy to attack.')
                choices = []
                for i in range(len(enemies)):
                    if choice == 0:
                        damage = max(player.attack - enemies[i].defense, 0)
                    else:
                        damage = max(calcrangedmg(player.primaryarrow) - enemies[i].defense, 0)
                    choices.append(enemies[i].name + ' (' + str(damage) + ' Damage)')
                enemy = getchoice(choices)
            if choice == 0:
                damage = max(player.attack - enemies[enemy].defense, 0)
            else:
                player.arrows[player.primaryarrow] -= 1
                damage = max(calcrangedmg(player.primaryarrow) - enemies[enemy].defense, 0)
            print('You dealt', damage, 'damage to', enemies[enemy].name + '.')
            if player.bloodthirster != 0: # 吸血
                obtained_hp = int(damage * player.bloodthirster / 100) #
                player.health += obtained_hp #
                print(f"---The Bloodthirster: Health + {obtained_hp}.---")#
            if player.critical_strike != 0 and enemies[enemy].health > 0:
                critical_random_num = rn(1, 100)
                if critical_random_num <= player.critical_strike:
                    critical_damage = int(damage * rn(70, 150) / 100)
                    critical_damage = min(enemies[enemy].health, critical_damage)
                    print(f"---Critical strike: You dealt extra {critical_damage} damage to {enemies[enemy].name}.---")#
            enemies[enemy].health -= damage
            if enemies[enemy].health <= 0:
                print(enemies[enemy].wininfo)
                enemies.pop(enemy)
        else:
            if random.random() < fleechance:
                fled = True
                print('Safe! You fled the battle.')
                player.attack = temp_attack
                player.defense = temp_defense
                break
            else:
                print('You was caught by the enemies and failed to flee.')
        if len(enemies) == 0:
                break
        printheader("Enemy's Turn")
        damage = 0
        for i in range(len(enemies)):
            if choice == 1 and enemies[i].ranged == False:
                continue
            damage += max(enemies[i].attack - player.defense, 0)
        if len(enemies) > 1:
            print('You received', damage, 'damage from enemies.')
        else:
            print('You received', damage, 'damage from', enemies[0].name + '.')
        if player.bless_forest:
            player.health -= int(damage * 0.3)
        else:
            player.health -= damage
        if player.health <= 0:
            if choice == 1:
                print(rangedfi)
            else:
                print(meleefi)
            break
        if player.swordsmanship is True:  # wugong
            player.attack = int(player.attack * 1.03)
            print("---Storm Sowrdsmanship: 3% extra attack obtained.---")
        if player.guardian is True:  # wugong
            player.defense = int(player.defense * 1.03)
            print("---Realm's Guardian: 3% extra defense fortified.---")
        if player.bless_forest is True: # Bless of The Children of The Forest
            print(f"---Blessed by The Children of The Forest: regenerates health after taking damage.---")
        if autoattack:
            wait(0.5)
        else:
            pause()
        clear()
        rounds += 1
    if rounds > maxrounds:
        player.health = 0
    if not fled and player.health > 0:
        print('Congratulations! You get the loot of ', end = '')
        if player.swordsmanship is True:
            player.attack = temp_attack  # wugong
        if player.guardian is True:
            player.defense = temp_defense  # wugong
        first = True
        for i in range(len(rewards)):
            if rewards[i] > 0:
                if first:
                    print(rewards[i], matnames[i], end = '')
                    first = False
                else:
                    print(',', rewards[i], matnames[i], end = '')
        if not first:
            print('!')
        else:
            print('nothing.')
        player.materials = list(map(operator.add, player.materials, rewards))

class Map:
    def __init__(self, rows, cols, events):
        self.rows = rows
        self.cols = cols
        self.playerx = rows // 2
        self.playery = cols // 2
        self.m = [[0] * cols for i in range(rows)]
        for i in range(rows):
            for j in range(cols):
                self.m[i][j] = 1 + rn(0, 2)
        self.m[self.playerx][self.playery] = 0
        self.visited = [[False] * cols for i in range(rows)]
        self.visited[self.playerx][self.playery] = True
        self.playersrec = {}
        for i in range(len(events)):
            for j in range(events[i]):
                r = rn(rows - 1)
                c = rn(cols - 1)
                while not (self.m[r][c] >= 1 and self.m[r][c] <= 3):
                    r = rn(rows - 1)
                    c = rn(cols - 1)
                self.m[r][c] = i + 4
    def printmap(self, bg = False):
        if not bg:
            print('X:', self.playerx, '   Y:', self.playery)
        print('#' * (self.cols * 2 + 2))
        for i in range(self.rows):
            print('#', end = '')
            for j in range(self.cols):
                if not bg and i == self.playerx and j == self.playery:
                    print('\33[0m\33[7mP \33[0m', end = '')
                    continue
                if bg and (i, j) in self.playersrec and self.playersrec[(i, j)] > 0:
                    print('\33[0m\33[7mP' + str(self.playersrec[(i, j)]) + '\33[0m', end = '')
                    continue
                if self.visited[i][j] or i - 1 >= 0 and self.visited[i - 1][j] \
                    or j - 1 >= 0 and self.visited[i][j - 1] \
                    or i + 1 < self.rows and self.visited[i + 1][j] \
                    or j + 1 < self.cols and self.visited[i][j + 1]:
                    if self.m[i][j] == 0:
                        print('\33[90m. ', end = '')
                    if self.m[i][j] >= 1 and self.m[i][j] <= 3:
                        print('\33[93mE', self.m[i][j], sep = '', end = '')
                    if self.m[i][j] == 4:
                        print('\33[92mW ', end = '')
                    if self.m[i][j] == 5:
                        print('\33[35m? ', end = '')
                    if self.m[i][j] == 6:
                        print('\33[91mB ', end = '')
                    if self.m[i][j] == 7:
                        print('\33[32mS ', end = '')
                else:
                    print('  ', end = '')
            print('\33[0m#')
        print('#' * (self.cols * 2 + 2))
    def move(self): # prompts the player to move, returns the event of the room which the player moved to
        dirx = [0, 1, -1, 0]
        diry = [-1, 0, 0, 1]
        while True:
            print("""      ┏━━┓   You can use WSAD or the number below to move
      ┃W^┃ 
      ┗━━┛
  ┏━━┓┏━━┓┏━━┓
  ┃<A┃┃Sˇ┃┃D>┃ 
  ┗━━┛┗━━┛┗━━┛
            """)
            direction = getchoice(('⬅ Left', '⬇ Down', '⬆ Up', '➡ Right'), getmove = True)
            nextx = self.playerx + dirx[direction]
            nexty = self.playery + diry[direction]
            if nextx >= 0 and nextx < self.rows and nexty >= 0 and nexty < self.cols:
                self.m[self.playerx][self.playery] = 0
                self.playerx = nextx
                self.playery = nexty
                self.visited[self.playerx][self.playery] = True
                break
            else:
                print('Invalid move')
        return self.m[self.playerx][self.playery]
