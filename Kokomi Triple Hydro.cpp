#include <stdio.h>
#include <string.h>

double hydroRes = 0.9; // hydro
double enemyRes = 0.9; // normal
double enemyDef = 0.5;

double prevFanfare = 0.0;
double fanfare = 0.0;
double totalDamage = 0.0;

int activeChar = 1;
double yelanBurstDMGBonus = 0.0;
double yelanDMGBuff = 0.0;
double kokomiOHCHeal = 0.0;

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
	yelanBurstDMGBonus = 25.0/100 * yelan.ER + yelan.DMGBonus;
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
	
	prevFanfare = fanfare;
	fanfare += 4 * 1.6;
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
	
	prevFanfare = fanfare;
	fanfare += 4 * 2.4;
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
	
	prevFanfare = fanfare;
	fanfare += 4 * 3.6;
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
	
	kokomiOHCHeal += (kokomi.HP * 7.9/100 + 932) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 7.9/100 + 932) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 7.9/100 + 932) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 7.9/100 + 932) * (1 + kokomi.HB + kokomi.incomingHB);
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
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
	
	kokomiOHCHeal += (kokomi.HP * 7.9/100 + 932) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 7.9/100 + 932) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 7.9/100 + 932) * (1 + kokomi.HB + kokomi.incomingHB);
	kokomiOHCHeal += (kokomi.HP * 7.9/100 + 932) * (1 + kokomi.HB + kokomi.incomingHB);
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
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
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
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
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
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
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.27/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

int main(){
	insertData();
	
	
	//0.02 kokomi n1
	kokomi.damage += (123.1/100 * kokomi.ATK) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (123.1/100 * kokomi.ATK) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	//0.28 kazuha skill
	activeChar = 3;
	kazuhaSkill();
	kokomi.DMGBonus += kazuha.EM * 0.04/100;
	yelan.DMGBonus += kazuha.EM * 0.04/100;
	furina.DMGBonus += kazuha.EM * 0.04/100;
	hydroRes = 1.15;
	
	//0.68 kazuha plunge
	kazuhaPlunge();
	
	//1.72 kokomi skill
	activeChar = 1;
	
	//2.12 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//2.93 furina skill
	activeChar = 4;
	
	//3.30 furina burst (cast)
	
	//4.22 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//4.53 furina seahorse
	furinaSeahorse();
	
	//5.02 furina octopus
	furinaOctopus();
	
	//5.02 furina crab
	furinaCrab();
	
	//5.12 kokomi ohc
	kokomiOHCHeal > 30000 ? kokomiOHCHeal = 30000 : kokomiOHCHeal = kokomiOHCHeal;
	kokomi.damage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	totalDamage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	
	//5.28 yelan skill
	activeChar = 2;
	yelan.damage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	
	//5.30 furina burst
	furina.damage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus) * hydroRes * enemyDef;
	prevFanfare = 0;
	fanfare = 0;
	
	//5.62 kokomi ohc start
	kokomiOHCHeal = 0.0;
	
	//5.97 yelan burst (cast)
	
	//6.16 furina seahorse
	furinaSeahorse();
	
	//6.22 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//7.23 yelan burst
	yelan.damage += (13.15/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	totalDamage += (13.15/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	
	yelanDMGBuff = 1.0/100;
	yelan.DMGBonus += yelanDMGBuff;
	
	//7.52 yelan n1 (cast)
	
	//7.73 yelan skill
	yelan.damage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	
	//7.87 furina seahorse
	furinaSeahorse();
	
	//7.93 yelan dice
	yelanDice();
	
	//8.22 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//8.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//8.25 furina octopus
	furinaOctopus();
	
	//8.60 kazuha skill
	activeChar = 3;
	yelan.DMGBonus -= yelanDMGBuff;
	kazuha.DMGBonus += yelanDMGBuff;
	kazuhaSkill();
	
	//8.62 kokomi ohc
	kokomiOHCHeal > 30000 ? kokomiOHCHeal = 30000 : kokomiOHCHeal = kokomiOHCHeal;
	kokomi.damage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	totalDamage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	
	//8.75 yelan dice
	yelanDice();
	
	//9.00 kazuha plunge
	kazuhaPlunge();
	
	//9.12 kokomi ohc start
	kokomiOHCHeal = 0.0;
	
	//9.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//9.47 furina seahorse
	furinaSeahorse();
	
	//10.03 kokomi burst (cast)
	activeChar = 1;
	kazuha.DMGBonus -= yelanDMGBuff;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//10.15 furina crab
	furinaCrab();
	
	//10.22 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//10.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//10.85 kokomi burst
	kokomi.damage += (18.7/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (18.7/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	//11.18 furina seahorse
	furinaSeahorse();
	
	//11.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//11.55 kokomi n1
	kokomiN1();
	
	//11.56 furina octopus
	furinaOctopus();
	
	//11.73 yelan dice
	yelanDice();
	
	//11.92 kokomi n2
	kokomiN2();
	
	//12.12 kokomi ohc
	kokomiOHCHeal > 30000 ? kokomiOHCHeal = 30000 : kokomiOHCHeal = kokomiOHCHeal;
	kokomi.damage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	totalDamage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	
	//12.22 kokomi jellyfish
	kokomiJellyfish();
	
	//12.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//12.62 kokomi ohc start
	kokomiOHCHeal = 0.0;
	
	//12.68 kokomi n3
	kokomiN3();
	
	//12.73 yelan dice
	yelanDice();
	
	//12.77 furina seahorse
	furinaSeahorse();
	
	//13.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//13.37 kokomi n1
	kokomiN1();
	
	//13.73 kokomi n2, yelan dice
	kokomiN2();
	yelanDice();
	
	//14.22 kokomi jellyfish
	kokomiJellyfish();
	
	//14.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//14.30 furina seahorse
	furinaSeahorse();
	
	//14.50 kokomi n3
	kokomiN3();
	
	//14.73 yelan dice
	yelanDice();
	
	//14.82 furina octopus
	furinaOctopus();
	
	//15.18 kokomi n1
	kokomiN1();
	
	//15.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//15.33 furina crab
	furinaCrab();
	
	//15.55 kokomi n2
	kokomiN2();
	
	//15.62 kokomi ohc
	kokomiOHCHeal > 30000 ? kokomiOHCHeal = 30000 : kokomiOHCHeal = kokomiOHCHeal;
	kokomi.damage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	totalDamage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	
	//15.73 yelan dice
	yelanDice();
	
	//15.94 furina crab
	furinaCrab();
	
	//16.12 kokomi ohc start
	kokomiOHCHeal = 0.0;
	
	//16.22 kokomi jellyfish
	kokomiJellyfish();
	
	//16.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//16.32 kokomi n3
	kokomiN3();
	
	//16.73 yelan dice
	yelanDice();
	
	//17.00 kokomi n1
	kokomiN1();
	kokomi.DMGBonus -= kazuha.EM * 0.04/100;
	yelan.DMGBonus -= kazuha.EM * 0.04/100;
	furina.DMGBonus -= kazuha.EM * 0.04/100;
	hydroRes = 0.9;
	
	//17.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//17.37 kokomi n2
	kokomiN2();
	
	//17.54 furina seahorse
	furinaSeahorse();
	
	//17.73 yelan dice
	yelanDice();
	
	//18.02 furina octopus
	furinaOctopus();
	
	//18.13 kokomi n3
	kokomiN3();
	
	//18.22 kokomi jellyfish
	kokomiJellyfish();
	
	//18.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//18.73 yelan dice
	yelanDice();
	
	//18.82 kokomi n1
	kokomiN1();
	
	//19.12 kokomi ohc
	kokomiOHCHeal > 30000 ? kokomiOHCHeal = 30000 : kokomiOHCHeal = kokomiOHCHeal;
	kokomi.damage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	totalDamage += kokomiOHCHeal * 0.9 * enemyRes * enemyDef;
	
	//19.13 furina seahorse
	furinaSeahorse();
	
	//19.18 kokomi n2
	kokomiN2();
	
	//19.23 yelan buff iteration
	yelanDMGBuff += 3.5/100;
	
	//19.73 yelan dice
	yelanDice();
	
	//19.95 kokomi n3
	kokomiN3();
	
	
	viewData();
	
	printf("Fanfare: %.1f\n", fanfare);
	printf("Total Damage: %.1f\n", totalDamage);
	printf("Kokomi Damage: %.1f\n", kokomi.damage);
	printf("Yelan Damage: %.1f\n", yelan.damage);
	printf("Kazuha Damage: %.1f\n", kazuha.damage);
	printf("Furina Damage: %.1f\n", furina.damage);
	
	puts("");
	printf("Damage per Second: %.1f\n", totalDamage / 20.0);
	printf("Kokomi DPS: %.1f\n", kokomi.damage / 20.0);
	printf("Yelan DPS: %.1f\n", yelan.damage / 20.0);
	printf("Kazuha DPS: %.1f\n", kazuha.damage / 20.0);
	printf("Furina DPS: %.1f\n", furina.damage / 20.0);
	
	puts("");
	printf("Kokomi Contribution: %.1f%%\n", kokomi.damage / totalDamage * 100);
	printf("Yelan Contribution: %.1f%%\n", yelan.damage / totalDamage * 100);
	printf("Kazuha Contribution: %.1f%%\n", kazuha.damage / totalDamage * 100);
	printf("Furina Contribution: %.1f%%\n", furina.damage / totalDamage * 100);
	
    return 0;
}
