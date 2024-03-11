#include <stdio.h>
#include <string.h>

double hydroRes = 0.9; // hydro
double enemyRes = 0.9; // normal
double enemyDef = 0.5;

double totalDamage = 0.0;
double prevFanfare = 0.0;
double fanfare = 0.0;

int activeChar = 1;

double kokomiOHCHeal = 0.0;
double yelanBurstDMGBonus = 0.0;
double yelanDMGBuff = 0.0;
bool furinaBurstActive = false;

struct {
	char name[7];
	double HP;
	double ATK;
	double ER;
	double EM;
	double DMGBonus;
	double HB;
	double incomingHB;
	double currHP;
	double prevHP;
	double critRate;
	double critDMG;
	double damage;
} kokomi, yelan, kazuha, furina;

void insertData(){
	strcpy(kokomi.name, "Kokomi"); // 40k hp | sig, 4p ohc, HP/Hydro/HB, 10/10/10
	kokomi.HP = 13471 * (1 + 49.6/100 + 46.6/100 + 17.5/100 + 17.5/100 + 10.5/100 + 25.0/100) + 4780; // base + sig + hp sands
																					// substat + substat + substat + hydro reso
	kokomi.ATK = (234 + 608) * (1 + 12.8/100.0) + 311 + 49; // base + sig + substat
	kokomi.DMGBonus = 46.6/100.0 + 28.8/100.0; // hydro gob + ascension
	kokomi.currHP = 100.0/100.0;
	kokomi.HB = 25.0/100.0 + 10.0/100 + 35.9/100 + 15.0/100; // base + sig + hb circlet + 2p ohc
	kokomi.incomingHB = 0.0/100.0;
	
	strcpy(yelan.name, "Yelan"); // 36k hp, 181.3 cv | fav, 4p eosf, HP/Hydro/Crit, 1/9/10 
	yelan.HP = 14450 * (1 + 46.6/100 + 13.4/100 + 15.2/100 + 4.1/100 + 25.0/100 + 12.0/100) + 4780 + 747; // base + hp sands + substat
																			// substat + substat + hydro reso + 2-element a1 passive
	yelan.ER = 100.0/100 + 61.3/100 + 20.0/100 + 5.8/100 + 6.5/100; // fav bow + 2p eosf + substat + substat
	yelan.DMGBonus = 46.6/100.0; // hydro gob
	yelan.critRate = 5/100.0 + 19.2/100.0 + 3.1/100.0 + 7.0/100 + 10.9/100 + 11.7/100; // base + ascension + substat + substat + substat
																					   // substat
	yelanBurstDMGBonus = 25.0/100 * yelan.ER;
	yelan.critDMG = 50/100.0 + 21.0/100.0 + 25.7/100 + 7.0/100 + 62.2/100; // base + substat + substat + substat + cd circlet
	yelan.currHP = 100.0/100.0;
	yelan.HB = 0.0/100.0;
	yelan.incomingHB = 0.0/100.0;
	
	strcpy(kazuha.name, "Kazuha"); // 756 em | fav, 4p vv, EM/EM/EM, 9/9/9
	kazuha.ATK = (297 + 454) * (1 + 5 * 4.955/100) + 311; // base + fav sword + substat + substat + substat + substat + substat
	kazuha.EM = 115 + 3 * 187 + 40 + 40; // base + em sands + em gob + em circlet + substat + substat
	kazuha.DMGBonus = 15.0/100; // 2p vv
	kazuha.critRate = 30.0/100; // base + substat
	kazuha.critDMG = 70.0/100; // base + substat
	kazuha.currHP = 100.0/100.0;
	kazuha.HB = 0.0/100.0;
	kazuha.incomingHB = 0.0/100.0;
	
	strcpy(furina.name, "Furina"); // 41k hp, 204.3 cv | fleuve cendre, 4p gt, HP/HP/Crit, 1/9/9
	furina.HP = 15307 * (1 + (46.6 + 46.6 + 20 + 25)/100) + 4780;
	furina.DMGBonus = 28.0/100.0 + 70/100.0; // a4 passive + 4p gt
	furina.critRate = 5/100.0 + 19.2/100.0 + 36.9/100.0 + 16.0/100; // base + ascension + substat + fontaine fishing sword
	furina.critDMG = 50/100.0 + 62.2/100.0 + 68.3/100.0; // base + cd circlet + substat
	furina.currHP = 100.0/100.0;
	furina.HB = 0.0/100.0;
	furina.incomingHB = 0.0/100.0;
}

void viewData(){
	printf("%s HP: %.0f\n", kokomi.name, kokomi.HP);
	printf("%s ATK: %.0f\n", kokomi.name, kokomi.ATK);
	printf("%s DMG Bonus: %.1f%%\n", kokomi.name, kokomi.DMGBonus * 100);
	printf("%s Current HP: %.1f%%\n", kokomi.name, kokomi.currHP * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", kokomi.name, kokomi.incomingHB * 100);
	
	puts("");
	
	printf("%s HP: %.0f\n", yelan.name, yelan.HP);
	printf("%s ER: %.1f%%\n", yelan.name, yelan.ER * 100);
	printf("%s DMG Bonus: %.1f%%\n", yelan.name, yelan.DMGBonus * 100);
	printf("%s Burst DMG Bonus: %.1f%%\n", yelan.name, yelanBurstDMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", yelan.name, yelan.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", yelan.name, yelan.critDMG * 100);
	printf("%s Current HP: %.1f%%\n", yelan.name, yelan.currHP * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", yelan.name, yelan.incomingHB * 100);
	
	puts("");
	
	printf("%s ATK: %.0f\n", kazuha.name, kazuha.ATK);
	printf("%s EM: %.0f\n", kazuha.name, kazuha.EM);
	printf("%s DMG Bonus: %.1f%%\n", kazuha.name, kazuha.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", kazuha.name, kazuha.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", kazuha.name, kazuha.critDMG * 100);
	printf("%s Current HP: %.1f%%\n", kazuha.name, kazuha.currHP * 100);
	printf("%s Healing Bonus: %.1f%%\n", kazuha.name, kazuha.HB * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", kazuha.name, kazuha.incomingHB * 100);
	
	puts("");
	
	printf("%s HP: %.0f\n", furina.name, furina.HP);
	printf("%s DMG Bonus: %.1f%%\n", furina.name, furina.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", furina.name, furina.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", furina.name, furina.critDMG * 100);
	printf("%s Current HP: %.1f%%\n", furina.name, furina.currHP * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", furina.name, furina.incomingHB * 100);
	
	puts("");
}

void furinaSeahorse(){
	furina.damage += (5.49/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * hydroRes * enemyDef;
	totalDamage += (5.49/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * hydroRes * enemyDef;
	kokomi.prevHP = kokomi.currHP;
	yelan.prevHP = yelan.currHP;
	kazuha.prevHP = kazuha.currHP;
	furina.prevHP = furina.currHP;
	kokomi.currHP -= 1.6/100;
	yelan.currHP -= 1.6/100;
	kazuha.currHP -= 1.6/100;
	furina.currHP -= 1.6/100;
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 4 * 1.6;
	
	if(fanfare > 300) fanfare = 300;
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void furinaOctopus(){
	furina.damage += (10.13/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * hydroRes * enemyDef;
	totalDamage += (10.13/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * hydroRes * enemyDef;
	kokomi.prevHP = kokomi.currHP;
	yelan.prevHP = yelan.currHP;
	kazuha.prevHP = kazuha.currHP;
	furina.prevHP = furina.currHP;
	kokomi.currHP -= 2.4/100;
	yelan.currHP -= 2.4/100;
	kazuha.currHP -= 2.4/100;
	furina.currHP -= 2.4/100;
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 4 * 2.4;
	
	if(fanfare > 300) fanfare = 300;
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void furinaCrab(){
	furina.damage += (14.09/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * hydroRes * enemyDef;
	totalDamage += (14.09/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * hydroRes * enemyDef;
	kokomi.prevHP = kokomi.currHP;
	yelan.prevHP = yelan.currHP;
	kazuha.prevHP = kazuha.currHP;
	furina.prevHP = furina.currHP;
	kokomi.currHP -= 3.6/100;
	yelan.currHP -= 3.6/100;
	kazuha.currHP -= 3.6/100;
	furina.currHP -= 3.6/100;
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 4 * 3.6;
	
	if(fanfare > 300) fanfare = 300;
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void kazuhaSkill(){
	// initial skill
	kazuha.damage += (443.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (443.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	
	// hydro swirl
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes * enemyDef;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes * enemyDef;
}

void kazuhaDot(){
	// anemo dot
	kazuha.damage += (204.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (204.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	
	// hydro dot
	kazuha.damage += (61.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (61.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * hydroRes * enemyDef;
	
	// hydro swirl
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes * enemyDef;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes * enemyDef;
}

void kazuhaPlunge(){
	// anemo dmg
	kazuha.damage += (349.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (349.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	
	// additional hydro dmg
	kazuha.damage += (200.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (200.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * hydroRes * enemyDef;
	
	// hydro swirl
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes * enemyDef;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes * enemyDef;
}

void kokomiJellyfishUnbuffed(){
	kokomi.damage += (196.5/100 * kokomi.ATK) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (196.5/100 * kokomi.ATK) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	kokomi.prevHP = kokomi.currHP;
	yelan.prevHP = yelan.currHP;
	kazuha.prevHP = kazuha.currHP;
	furina.prevHP = furina.currHP;
	
	switch (activeChar){
		case 1:
			if((kokomi.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kokomi.incomingHB) > 100.0/100) kokomi.currHP = 100.0/100;
			else kokomi.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kokomi.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kokomi.incomingHB);
			break;
		case 2:
			if((yelan.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB) > 100.0/100) yelan.currHP = 100.0/100;
			else yelan.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB);
			break;
		case 3:
			if((kazuha.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB) > 100.0/100) kazuha.currHP = 100.0/100;
			else kazuha.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB);
			break;
		case 4:
			if((furina.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB) > 100.0/100) furina.currHP = 100.0/100;
			else furina.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB);
			break;
		default:
			break;
	}
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void kokomiJellyfish(){
	kokomi.damage += (196.5/100 * kokomi.ATK + 12.8/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (196.5/100 * kokomi.ATK + 12.8/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	kokomi.prevHP = kokomi.currHP;
	yelan.prevHP = yelan.currHP;
	kazuha.prevHP = kazuha.currHP;
	furina.prevHP = furina.currHP;
	
	switch (activeChar){
		case 1:
			if((kokomi.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kokomi.incomingHB) > 100.0/100) kokomi.currHP = 100.0/100;
			else kokomi.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kokomi.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kokomi.incomingHB);
			break;
		case 2:
			if((yelan.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB) > 100.0/100) yelan.currHP = 100.0/100;
			else yelan.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB);
			break;
		case 3:
			if((kazuha.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB) > 100.0/100) kazuha.currHP = 100.0/100;
			else kazuha.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB);
			break;
		case 4:
			if((furina.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB) > 100.0/100) furina.currHP = 100.0/100;
			else furina.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB);
			break;
		default:
			break;
	}
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void yelanDice(){
	yelan.damage += 3 * (8.77/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	totalDamage += 3 * (8.77/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
}

void kokomiN1(){
	kokomi.damage += (123.1/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 1.0/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (123.1/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 1.0/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	kokomi.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	if(kokomi.currHP > 1) kokomi.currHP = 100.0/100.0;
	yelan.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + yelan.incomingHB);
	if(yelan.currHP > 1) yelan.currHP = 100.0/100.0;
	kazuha.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kazuha.incomingHB);
	if(kazuha.currHP > 1) kazuha.currHP = 100.0/100.0;
	furina.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + furina.incomingHB);
	if(furina.currHP > 1) furina.currHP = 100.0/100.0;
	
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + yelan.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kazuha.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + furina.incomingHB);
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void kokomiN2(){
	kokomi.damage += (110.8/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 1.0/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (110.8/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 1.0/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	kokomi.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	if(kokomi.currHP > 1) kokomi.currHP = 100.0/100.0;
	yelan.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + yelan.incomingHB);
	if(yelan.currHP > 1) yelan.currHP = 100.0/100.0;
	kazuha.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kazuha.incomingHB);
	if(kazuha.currHP > 1) kazuha.currHP = 100.0/100.0;
	furina.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + furina.incomingHB);
	if(furina.currHP > 1) furina.currHP = 100.0/100.0;
	
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + yelan.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kazuha.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + furina.incomingHB);
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void kokomiN3(){
	kokomi.damage += (169.8/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 1.0/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (169.8/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 1.0/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	kokomi.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	if(kokomi.currHP > 1) kokomi.currHP = 100.0/100.0;
	yelan.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + yelan.incomingHB);
	if(yelan.currHP > 1) yelan.currHP = 100.0/100.0;
	kazuha.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kazuha.incomingHB);
	if(kazuha.currHP > 1) kazuha.currHP = 100.0/100.0;
	furina.currHP += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + furina.incomingHB);
	if(furina.currHP > 1) furina.currHP = 100.0/100.0;
	
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + yelan.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kazuha.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + furina.incomingHB);
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void kokomiOHC(){
	kokomiOHCHeal > 30000 ? kokomiOHCHeal = 30000 : kokomiOHCHeal = kokomiOHCHeal;
	kokomi.damage += kokomiOHCHeal * 0.9 * enemyRes;
	totalDamage += kokomiOHCHeal * 0.9 * enemyRes;
}

int main(){
	insertData();
	
	
	//0.02 kokomi skill
	activeChar = 1;
	
	//0.42 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//1.23 kazuha skill
	activeChar = 3;
	kazuhaSkill();
	kokomi.DMGBonus += kazuha.EM * 0.04/100;
	yelan.DMGBonus += kazuha.EM * 0.04/100;
	furina.DMGBonus += kazuha.EM * 0.04/100;
	hydroRes = 1.15;
	
	//1.63 kazuha plunge
	kazuhaPlunge();
	
	//2.67 furina skill
	activeChar = 4;
	
	//3.03 furina burst (cast)
	
	//3.42 kokomi ohc
	kokomiOHC();
	
	//3.92 kokomi ohc start
	kokomiOHCHeal = 0.0;
	
	//4.27 furina seahorse
	furinaSeahorse();
	
	//4.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//4.76 furina octopus, furina crab
	furinaOctopus();
	furinaCrab();
	
	//4.55 furina burst
	furina.damage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus) * hydroRes * enemyDef;
	furinaBurstActive = true;
	prevFanfare = 0;
	fanfare = 0;
	
	//5.62 yelan skill
	activeChar = 2;
	yelan.damage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	
	//5.70 yelan burst (cast)
	
	//5.90 furina seahorse
	furinaSeahorse();
	
	//6.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//6.92 kokomi ohc
	kokomiOHC();
	
	//6.97 yelan burst
	yelan.damage += (13.15/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	totalDamage += (13.15/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	
	yelanDMGBuff = 1.0/100;
	yelan.DMGBonus += yelanDMGBuff;
	
	//7.25 yelan n1 (cast)
	
	//7.42 kokomi ohc start
	kokomiOHCHeal = 0.0;
	
	//7.61 furina seahorse
	furinaSeahorse();
	
	//7.67 yelan dice
	yelanDice();
	
	//7.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//7.99 furina octopus
	furinaOctopus();
	
	//8.07 yelan skill
	yelan.damage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	
	//8.48 yelan dice
	yelanDice();
	
	//8.50 kazuha skill
	activeChar = 3;
	yelan.DMGBonus -= yelanDMGBuff;
	kazuha.DMGBonus += yelanDMGBuff;
	kazuhaSkill();
	
	//8.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//8.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//9.21 furina seahorse
	furinaSeahorse();
	
	//9.33 kazuha plunge
	kazuhaPlunge();
	
	//9.65 kazuha burst (cast)
	
	//9.89 furina crab
	furinaCrab();
	
	//9.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//10.42 kokomi ohc
	kokomiOHC();
	
	//10.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//10.92 kokomi ohc start, furina seahorse
	kokomiOHCHeal = 0.0;
	furinaSeahorse();
	
	//10.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//11.02 kazuha burst
	kazuha.damage += (446.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (446.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes * enemyDef;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes * enemyDef;
	
	//11.30 furina octopus
	furinaOctopus();
	
	//11.45 kokomi burst (cast)
	activeChar = 1;
	kazuha.DMGBonus -= yelanDMGBuff;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//11.62 kokomi n1 (hit)
	kokomi.damage += (123.1/100 * kokomi.ATK) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (123.1/100 * kokomi.ATK) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	//11.80 yelan dice
	yelanDice();
	
	//11.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//12.03 kazuha dot
	kazuhaDot();
	
	//12.27 kokomi burst
	kokomi.damage += (18.7/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (18.7/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
		
	//12.51 furina seahorse
	furinaSeahorse();
	
	//12.52 kokomi jellyfish
	kokomiJellyfish();
	
	//12.97 kokomi n1, yelan buff iteration
	kokomiN1();
	yelanDMGBuff += 3.5/100;
	
	//13.15 yelan dice
	yelanDice();
	
	//13.33 kokomi n2
	kokomiN2();
	
	//13.73 kokomi n1
	kokomiN1();
	
	//13.92 kokomi ohc
	kokomiOHC();
	
	//13.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//13.98 kazuha dot
	kazuhaDot();
	
	//14.04 furina seahorse
	furinaSeahorse();
	
	//14.10 kokomi n2
	kokomiN2();
	
	//14.15 yelan dice
	yelanDice();
	
	//14.42 kokomi ohc start
	kokomiOHCHeal = 0.0;
	
	//14.50 kokomi n1
	kokomiN1();
	
	//14.52 kokomi jellyfish
	kokomiJellyfish();
	
	//14.56 furina octopus
	furinaOctopus();
	
	//14.87 kokomi n2
	kokomiN2();
	
	//14.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//15.07 furina crab
	furinaCrab();
	
	//15.15 yelan dice
	yelanDice();
	
	//15.68 furina seahorse
	furinaSeahorse();
	
	//15.93 kazuha dot
	kazuhaDot();
	
	//15.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//16.00 kokomi n1
	kokomiN1();
	
	//16.18 yelan dice
	yelanDice();
	
	//16.37 kokomi n2
	kokomiN2();
	
	//16.52 kokomi jellyfish
	kokomiJellyfish();
	
	//16.77 kokomi n1
	kokomiN1();
	
	//16.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//17.13 kokomi n2
	kokomiN2();
	
	//17.18 yelan dice
	yelanDice();
	
	//17.28 furina seahorse
	furinaSeahorse();
	
	//17.42 kokomi ohc
	kokomiOHC();
	
	//17.76 furina octopus
	furinaOctopus();
	
	//17.88 kazuha dot
	kazuhaDot();
	
	//17.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//18.27 kokomi n1
	kokomiN1();
	
	//18.45 yelan dice
	yelanDice();
	
	//18.52 kokomi jellyfish
	kokomiJellyfish();
	
	//18.63 kokomi n2
	kokomiN2();
	
	//18.87 furina seahorse
	furinaSeahorse();
	
	//18.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//19.03 kokomi n1
	kokomiN1();
	
	//19.40 kokomi n2
	kokomiN2();
	
	//19.45 yelan dice
	yelanDice();
	
	//19.83 kazuha dot
	kazuhaDot();
	
	//19.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//20.27 vv uptime wears off
	hydroRes = 0.9;
	
	//20.31 furina crab
	furinaCrab();
	
	//20.51 furina seahorse
	furinaSeahorse();
	
	//20.52 kokomi jellyfish
	kokomiJellyfish();
	
	//20.53 kokomi n1
	kokomiN1();
	
	//20.72 yelan dice
	yelanDice();
	
	//20.90 kokomi n2
	kokomiN2();
	
	//20.92 furina octopus
	furinaOctopus();
	
	//20.97 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//21.30 kokomi n1
	kokomiN1();
	
	//21.67 kokomi n2
	kokomiN2();
	
	//21.72 yelan dice
	yelanDice();
	
	
	viewData();
	
	printf("Fanfare: %.1f\n", fanfare);
	printf("Total Damage: %.1f\n", totalDamage);
	printf("Kokomi Damage: %.1f\n", kokomi.damage);
	printf("Yelan Damage: %.1f\n", yelan.damage);
	printf("Kazuha Damage: %.1f\n", kazuha.damage);
	printf("Furina Damage: %.1f\n", furina.damage);
	
	puts("");
	printf("Damage per Second: %.1f\n", totalDamage / 22.0);
	printf("Kokomi DPS: %.1f\n", kokomi.damage / 22.0);
	printf("Yelan DPS: %.1f\n", yelan.damage / 22.0);
	printf("Kazuha DPS: %.1f\n", kazuha.damage / 22.0);
	printf("Furina DPS: %.1f\n", furina.damage / 22.0);
	
	puts("");
	printf("Kokomi Contribution: %.1f%%\n", kokomi.damage / totalDamage * 100);
	printf("Yelan Contribution: %.1f%%\n", yelan.damage / totalDamage * 100);
	printf("Kazuha Contribution: %.1f%%\n", kazuha.damage / totalDamage * 100);
	printf("Furina Contribution: %.1f%%\n", furina.damage / totalDamage * 100);
	
    return 0;
}
