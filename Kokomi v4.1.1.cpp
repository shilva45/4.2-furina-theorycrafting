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
double kokomiBondOfLifeDebuff = 99999999.0;
int kokomiBotBBStacks = 0;
int kokomiUsingMoonglow = 0;
double yelanBurstDMGBonus = 0.0;
double yelanDMGBuff = 0.0;
bool furinaBurstActive = false;
bool furinaHPCheck = true;

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
//	strcpy(kokomi.name, "Kokomi"); // 35k hp, 1.6k atk | flowing purity r5, 4p ohc, HP/Hydro/HB, 10/10/10
//	kokomi.HP = 13471 * (1 + 46.6/100 + 0.496 + 25.0/100) + 4780 + 507.88; // base + hp sands
//																				// substat + hydro reso
//	kokomi.ATK = (234 + 565) * (1 + 27.6/100 + 0.3968) + 311 + 33.08; // base + flowing purity + substat
//	kokomi.ER = 1 + 0.4408; // base + substats
//	kokomi.DMGBonus = 46.6/100.0 + 28.8/100.0; // hydro gob + ascension
//	kokomi.currHP = 100.0/100.0;
//	kokomi.HB = 25.0/100.0 + 35.9/100 + 15.0/100; // base + hb circlet + 2p ohc
//	kokomi.incomingHB = 0.0/100.0;
//	
//	strcpy(kokomi.name, "Kokomi"); // 35k hp, 1.6k atk | botbb r5, 4p ohc, HP/Hydro/HB, 10/10/10
//	kokomi.HP = 13471 * (1 + 46.6/100 + 0.496 + 25.0/100) + 4780 + 761.82; // base + hp sands
//																				// substat + hydro reso
//	kokomi.ATK = (234 + 565) * (1 + 0.5952) + 311 + 33.08; // base + substat
//	kokomi.ER = 1 + 0.1653 + 30.6/100; // base + substats + botbb
//	kokomi.DMGBonus = 46.6/100.0 + 28.8/100.0; // hydro gob + ascension
//	kokomi.currHP = 100.0/100.0;
//	kokomi.HB = 25.0/100.0 + 35.9/100 + 15.0/100; // base + hb circlet + 2p ohc
//	kokomi.incomingHB = 0.0/100.0;
	
//	strcpy(kokomi.name, "Kokomi"); // 41k hp, 1.5k atk | pamber r5, 4p ohc, HP/Hydro/HB, 10/10/10
//	kokomi.HP = 13471 * (1 + 41.3/100 + 46.6/100 + 0.496 + 25.0/100) + 4780 + 1015.76; // base + pamber + hp sands
//																				// substat + hydro reso
//	kokomi.ATK = (234 + 510) * (1 + 0.5952) + 311 + 33.08; // base + substat
//	kokomi.ER = 1 + 0.1102; // base + substats
//	kokomi.DMGBonus = 46.6/100.0 + 28.8/100.0; // hydro gob + ascension
//	kokomi.currHP = 100.0/100.0;
//	kokomi.HB = 25.0/100.0 + 35.9/100 + 15.0/100; // base + hb circlet + 2p ohc
//	kokomi.incomingHB = 0.0/100.0;
	
	strcpy(kokomi.name, "Kokomi"); // 42k hp, 1.6k atk | moonglow r1, 4p ohc, HP/Hydro/HB, 10/10/10
	kokomi.HP = 13471 * (1 + 49.6/100 + 46.6/100 + 0.496 + 25.0/100) + 4780 + 1015.76; // base + moonglow + hp sands
																				// substat + hydro reso
	kokomi.ATK = (234 + 608) * (1 + 0.5952) + 311 + 33.08; // base + substat
	kokomi.ER = 1 + 0.1102; // base + substats
	kokomi.DMGBonus = 46.6/100.0 + 28.8/100.0; // hydro gob + ascension
	kokomi.currHP = 100.0/100.0;
	kokomi.HB = 25.0/100.0 + 10.0/100 + 35.9/100 + 15.0/100; // base + moonglow + hb circlet + 2p ohc
	kokomi.incomingHB = 0.0/100.0;
	kokomiUsingMoonglow = 1;
	
	
	
	strcpy(yelan.name, "Yelan"); // 33k hp, 174.7 cv | slingshot, 4p eosf, HP/Hydro/Crit, 1/9/9
	yelan.HP = 14450 * (1 + 46.6/100 + 0.0992 + 25.0/100 + 12.0/100) + 4780 + 507.88; // base + hp sands + substat
																			// hydro reso + 2-element a1 passive
	yelan.ER = 100.0/100 + 20.0/100 + 0.4959; // 2p eosf + substat
	yelan.DMGBonus = 46.6/100.0; // hydro gob
	yelan.critRate = 5/100.0 + 19.2/100.0 + 0.2979 + 31.2/100; // base + ascension + substat + slingshot
	yelanBurstDMGBonus = 25.0/100 * yelan.ER;
	yelan.critDMG = 50/100.0 + 62.2/100 + 0.5296; // base + cd circlet + substat
	yelan.currHP = 100.0/100.0;
	yelan.HB = 0.0/100.0;
	yelan.incomingHB = 0.0/100.0;
	
	strcpy(kazuha.name, "Kazuha"); // 795 em | fav, 4p vv, EM/EM/EM, 9/9/9
	kazuha.ATK = (297 + 454) * (1 + 0.0992) + 311 + 33.08; // base + fav sword + substat
	kazuha.EM = 115 + 3 * 187 + 118.92; // base + em sands + em gob + em circlet + substat
	kazuha.DMGBonus = 15.0/100; // 2p vv
	kazuha.critRate = 5.0/100 + 0.3641; // base + substat
	kazuha.critDMG = 50.0/100 + 0.1324; // base + substat
	kazuha.currHP = 100.0/100.0;
	kazuha.HB = 0.0/100.0;
	kazuha.incomingHB = 0.0/100.0;
	
	strcpy(furina.name, "Furina"); // 39k hp, 174.7 cv | hod, 4p gt, HP/HP/Crit, 1/9/9
	furina.HP = 15307 * (1 + 46.6/100 + 46.6/100 + 25.0/100 + 0.0992) + 4780 + 507.88; // base + hp sands + hp gob + hydro reso + substat
	furina.ER = 1 + 0.4408;
	furina.DMGBonus = (0.7/100 * furina.HP/1000) < 28.0/100 ? (0.7/100 * furina.HP/1000) : 28.0/100; // a4 passive
	furina.DMGBonus += 70/100.0; // 4p gt
	furina.critRate = 5/100.0 + 19.2/100 + 0.3972 + 28.0/100; // base + ascension + substat + hod passive
	furina.critDMG = 50/100.0 + 62.2/100 + 0.3972 + 46.9/100; // base + cd circlet + substat + hod
	furina.currHP = 100.0/100.0;
	furina.HB = 0.0/100.0;
	furina.incomingHB = 0.0/100.0;
}

void viewData(){
	printf("%s HP: %.0f\n", kokomi.name, kokomi.HP);
	printf("%s ATK: %.0f\n", kokomi.name, kokomi.ATK);
	printf("%s ER: %.1f%%\n", kokomi.name, kokomi.ER * 100);
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
	printf("%s ER: %.1f%%\n", furina.name, furina.ER * 100);
	printf("%s DMG Bonus: %.1f%%\n", furina.name, furina.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", furina.name, furina.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", furina.name, furina.critDMG * 100);
	printf("%s Current HP: %.1f%%\n", furina.name, furina.currHP * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", furina.name, furina.incomingHB * 100);
	
	puts("");
}

void furinaHodCheck(){
	if(furina.currHP <= 90.0/100 && furinaHPCheck == true){
		furinaHPCheck = false;
		furina.critRate -= 28.0/100;
	} else if(furina.currHP > 90.0/100 && furinaHPCheck == false){
		furinaHPCheck = true;
		furina.critRate += 28.0/100;
	} else return;
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
	
	furinaHodCheck();
	
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
	
	furinaHodCheck();
	
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
	
	furinaHodCheck();
	
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
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
}

void kazuhaDot(){
	// anemo dot
	kazuha.damage += (204.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (204.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	
	// hydro dot
	kazuha.damage += (61.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (61.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * hydroRes * enemyDef;
	
	// hydro swirl
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
}

void kazuhaPlunge(){
	// anemo dmg
	kazuha.damage += (349.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (349.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	
	// additional hydro dmg
	kazuha.damage += (200.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (200.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * hydroRes * enemyDef;
	
	// hydro swirl
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
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
			kokomiBondOfLifeDebuff -= (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kokomi.incomingHB);
			break;
		case 2:
			if((yelan.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB) > 100.0/100) yelan.currHP = 100.0/100;
			else yelan.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB);
			kokomiBondOfLifeDebuff -= (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + yelan.incomingHB);
			break;
		case 3:
			if((kazuha.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB) > 100.0/100) kazuha.currHP = 100.0/100;
			else kazuha.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB);
			kokomiBondOfLifeDebuff -= (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kazuha.incomingHB);
			break;
		case 4:
			if((furina.currHP + 7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB) > 100.0/100) furina.currHP = 100.0/100;
			else furina.currHP += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB);
			kokomiOHCHeal += (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB);
			kokomiBondOfLifeDebuff -= (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + furina.incomingHB);
			break;
		default:
			break;
	}
	
	furinaHodCheck();
	
	if(kokomiBondOfLifeDebuff <= 0){
		kokomi.DMGBonus += 24.0/100;
		kokomiBondOfLifeDebuff = 999999999.0;
	}
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
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
			kokomiBondOfLifeDebuff -= (7.9/100 * kokomi.HP + 932) * (1 + kokomi.HB + kokomi.incomingHB);
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
	
	furinaHodCheck();
	
	if(kokomiBondOfLifeDebuff <= 0){
		kokomi.DMGBonus += 24.0/100;
		kokomiBondOfLifeDebuff = 999999999.0;
	}
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
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

void yelanDice(){
	yelan.damage += 3 * (8.77/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	totalDamage += 3 * (8.77/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
}

void kokomiN1(){
	kokomi.damage += (123.1/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus + 16.0/100 * kokomiBotBBStacks) * hydroRes * enemyDef;
	totalDamage += (123.1/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus + 16.0/100 * kokomiBotBBStacks) * hydroRes * enemyDef;
	
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
	
	kokomiBondOfLifeDebuff -= (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	
	if(kokomiBondOfLifeDebuff <= 0){
		kokomi.DMGBonus += 24.0/100;
		kokomiBondOfLifeDebuff = 999999999.0;
	}
	
	furinaHodCheck();
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
//	printf("%.1f, %.1f, %.1f, %.1f, %.1f\n", fanfare, kokomi.currHP * 100, yelan.currHP * 100, kazuha.currHP * 100, furina.currHP * 100);
	
	kokomi.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	yelan.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	kazuha.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	kokomi.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	yelan.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	kazuha.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void kokomiN2(){
	kokomi.damage += (110.8/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus + 16.0/100 * kokomiBotBBStacks) * hydroRes * enemyDef;
	totalDamage += (110.8/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus + 16.0/100 * kokomiBotBBStacks) * hydroRes * enemyDef;
	
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
	
	kokomiBondOfLifeDebuff -= (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	
	if(kokomiBondOfLifeDebuff <= 0){
		kokomi.DMGBonus += 24.0/100;
		kokomiBondOfLifeDebuff = 999999999.0;
	}
	
	furinaHodCheck();
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
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

void kokomiN3(){
	kokomi.damage += (169.8/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus + 16.0/100 * kokomiBotBBStacks) * hydroRes * enemyDef;
	totalDamage += (169.8/100 * kokomi.ATK + 8.7/100 * kokomi.HP + 15.0/100 * kokomi.HB * kokomi.HP + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus + 16.0/100 * kokomiBotBBStacks) * hydroRes * enemyDef;
	
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
	
	kokomiBondOfLifeDebuff -= (kokomi.HP * 1.45/100 + 169) * (1 + kokomi.HB + kokomi.incomingHB);
	
	if(kokomiBondOfLifeDebuff <= 0){
		kokomi.DMGBonus += 24.0/100;
		kokomiBondOfLifeDebuff = 999999999.0;
	}
	
	furinaHodCheck();
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (kokomi.currHP - kokomi.prevHP);
	fanfare += 100 * (yelan.currHP - yelan.prevHP);
	fanfare += 100 * (kazuha.currHP - kazuha.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
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

void kokomiOHC(){
	kokomiOHCHeal > 30000 ? kokomiOHCHeal = 30000 : kokomiOHCHeal = kokomiOHCHeal;
	kokomi.damage += kokomiOHCHeal * 0.9 * enemyRes;
	totalDamage += kokomiOHCHeal * 0.9 * enemyRes;
}

int main(){
	insertData();
	
	
	//0.02 kokomi skill (cast)
	activeChar = 1;
	
	//0.42 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
//	//0.42 kokomi (flowing purity r5) debuff
//	kokomiBondOfLifeDebuff = 24.0/100 * kokomi.HP;
//	kokomi.DMGBonus += 16.0/100;

	//1.45 kazuha skill
	activeChar = 3;
	kazuhaSkill();
	kokomi.DMGBonus += kazuha.EM * 0.04/100;
	yelan.DMGBonus += kazuha.EM * 0.04/100;
	furina.DMGBonus += kazuha.EM * 0.04/100;
	hydroRes = 1.15;
	
	//2.32 kazuha plunge
	kazuhaPlunge();
	
	//2.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//2.75 furina skill
	activeChar = 4;
	
	//3.12 furina burst (cast)
	
	//3.43 kokomi ohc pop
	kokomiOHC();
	
	//3.93 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	//4.34 furina seahorse
	furinaSeahorse();
	
	//4.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//4.63 furina burst
	furina.damage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus) * hydroRes * enemyDef;
	furinaBurstActive = true;
	prevFanfare = 0;
	fanfare = 0;
	
	//4.84 furina octopus, furina crab
	furinaOctopus();
	furinaCrab();
	
	//5.78 yelan skill
	activeChar = 2;
	yelan.damage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	
	//5.93 yelan burst (cast)
	
	//5.98 furina seahorse
	furinaSeahorse();
	
	//6.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//7.20 yelan burst
	yelan.damage += (13.15/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	totalDamage += (13.15/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	
	yelanDMGBuff = 1.0/100;
	yelan.DMGBonus += yelanDMGBuff;
	
	//7.52 yelan n1 (cast)
	
	//7.53 kokomi ohc pop
	kokomiOHC();
	
	//7.69 furina seahorse
	furinaSeahorse();
	
	//7.93 yelan dice
	yelanDice();
	
	//8.03 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	//8.07 furina octopus
	furinaOctopus();
	
	//8.17 kazuha skill
	activeChar = 3;
	yelan.DMGBonus -= yelanDMGBuff;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//8.20 yelan buff iteration
	kazuha.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//8.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//9.07 kazuha plunge
	kazuhaPlunge();
	
	//9.20 yelan buff iteration
	kazuha.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//9.29 furina seahorse
	furinaSeahorse();
	
	//9.38 kazuha n1 (cast)
	
	//9.78 kazuha burst (cast)
	
	//9.80 yelan dice
	yelanDice();
	
	//9.97 furina crab
	furinaCrab();
	
	//10.20 yelan buff iteration
	kazuha.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//10.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//11.00 furina seahorse
	furinaSeahorse();
	
	//11.15 kazuha burst
	kazuha.damage += (446.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (446.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
	
	//11.20 yelan buff iteration
	kazuha.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//11.38 furina octopus
	furinaOctopus();
	
	//11.53 kokomi ohc pop
	kokomiOHC();
	
	//11.58 kokomi n1 (cast)
	activeChar = 1;
	kazuha.DMGBonus -= yelanDMGBuff;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//11.73 kokomi burst (cast)
	
	//11.82 kokomi n1 (unbuffed)
	kokomi.damage += (123.1/100 * kokomi.ATK + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (123.1/100 * kokomi.ATK + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
//	kokomiBotBBStacks++;
	
	//12.00 yelan dice
	yelanDice();
	
	//12.03 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	//12.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//12.17 kazuha dot
	kazuhaDot();
	
	//12.52 kokomi jellyfish
	kokomiJellyfish();
	
	//12.55 kokomi burst
	kokomi.damage += (18.7/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (18.7/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	//12.59 furina seahorse
	furinaSeahorse();
	
	//13.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//13.30 kokomi n1
	kokomiN1();
//	kokomiBotBBStacks++;

	//13.48 yelan dice
	yelanDice();
	
	//13.67 kokomi n2
	kokomiN2();
//	kokomiBotBBStacks++;
	
	//14.12 furina seahorse
	furinaSeahorse();
	
	//14.12 kazuha dot
	kazuhaDot();
	
	//14.15 kokomi n1
	kokomiN1();
	
	//14.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//14.48 yelan dice
	yelanDice();
	
	//14.52 kokomi jellyfish
	kokomiJellyfish();
	
	//14.52 kokomi n2
	kokomiN2();
	
//	//13.25 kokomi prototype amber burst heal
//	kokomi.currHP += 6.0/100;
//	yelan.currHP += 6.0/100;
//	kazuha.currHP += 6.0/100;
//	furina.currHP += 6.0/100;
//	
//	if(kokomi.currHP > 1) kokomi.currHP = 1;
//	if(yelan.currHP > 1) yelan.currHP = 1;
//	if(kazuha.currHP > 1) kazuha.currHP = 1;
//	if(furina.currHP > 1) furina.currHP = 1;
	
	//14.64 furina octopus
	furinaOctopus();
	
	//15.00 kokomi n1
	kokomiN1();
	
	//15.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//15.15 furina crab
	furinaCrab();
	
	//15.37 kokomi n2
	kokomiN2();
	
//	//15.42 kokomi (flowing purity r5) initial dmg bonus buff expires
//	kokomi.DMGBonus -= 16.0/100;
	
	//15.48 yelan dice
	yelanDice();
	
	//15.53 kokomi ohc pop
	kokomiOHC();
	
	//15.76 furina seahorse
	furinaSeahorse();
	
	//15.85 kokomi n1
	kokomiN1();
	
	//16.03 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	//16.07 kazuha dot
	kazuhaDot();
	
	//16.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//16.22 kokomi n2
	kokomiN2();
	
	//16.52 kokomi jellyfish
	kokomiJellyfish();
	
//	//15.25 kokomi prototype amber burst heal
//	kokomi.currHP += 6.0/100;
//	yelan.currHP += 6.0/100;
//	kazuha.currHP += 6.0/100;
//	furina.currHP += 6.0/100;
//	
//	if(kokomi.currHP > 1) kokomi.currHP = 1;
//	if(yelan.currHP > 1) yelan.currHP = 1;
//	if(kazuha.currHP > 1) kazuha.currHP = 1;
//	if(furina.currHP > 1) furina.currHP = 1;
	
	//16.70 kokomi n1
	kokomiN1();
	
	//16.88 yelan dice
	yelanDice();
	
	//17.07 kokomi n2
	kokomiN2();
	
	//17.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//17.36 furina seahorse
	furinaSeahorse();
	
	//17.55 kokomi n1
	kokomiN1();
	
	//17.84 furina octopus
	furinaOctopus();
	
	//17.88 yelan dice
	yelanDice();
	
	//17.92 kokomi n2
	kokomiN2();
	
	//18.02 kazuha dot
	kazuhaDot();
	
	//18.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//18.38 kokomi n1
	kokomiN1();
	
	//18.52 kokomi jellyfish
	kokomiJellyfish();
	
//	//17.25 kokomi prototype amber burst heal
//	kokomi.currHP += 6.0/100;
//	yelan.currHP += 6.0/100;
//	kazuha.currHP += 6.0/100;
//	furina.currHP += 6.0/100;
//	
//	if(kokomi.currHP > 1) kokomi.currHP = 1;
//	if(yelan.currHP > 1) yelan.currHP = 1;
//	if(kazuha.currHP > 1) kazuha.currHP = 1;
//	if(furina.currHP > 1) furina.currHP = 1;
	
	//18.75 kokomi n2
	kokomiN2();
	
	//18.88 yelan dice
	yelanDice();
	
	//18.95 furina seahorse
	furinaSeahorse();
	
	//19.15 vv uptime wears off
	hydroRes = 0.9;
	
	//19.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//19.22 kokomi n1
	kokomiN1();
	
	//19.23 kokomi ohc pop
	kokomiOHC();
	
	//19.58 kokomi n2
	kokomiN2();
	
	//19.73 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	totalDamage = 0;
	kokomi.damage = 0;
	yelan.damage = 0;
	kazuha.damage = 0;
	furina.damage = 0;
	
	//20.02 kazuha dot
	kazuhaDot();
	
	//20.02 kokomi skill (cast)
	activeChar = 1;
	
	//20.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//20.39 furina crab
	furinaCrab();
	
	//20.42 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
//	//20.42 kokomi (flowing purity r5) debuff
//	kokomiBondOfLifeDebuff = 24.0/100 * kokomi.HP;
//	kokomi.DMGBonus += 16.0/100;
	
	//20.59 furina seahorse
	furinaSeahorse();
	
	//20.59 furina octopus
	furinaOctopus();
	
	//21.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;

	//21.45 kazuha skill
	activeChar = 3;
	kazuhaSkill();
	kokomi.DMGBonus += kazuha.EM * 0.04/100;
	yelan.DMGBonus += kazuha.EM * 0.04/100;
	furina.DMGBonus += kazuha.EM * 0.04/100;
	hydroRes = 1.15;
	
	//22.20 yelan buff expires
	kokomi.DMGBonus -= yelanDMGBuff;
	
	//22.20 furina seahorse
	furinaSeahorse();
	
	//22.32 kazuha plunge
	kazuhaPlunge();
	
	//22.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//22.63 furina burst expires
	furinaBurstActive = false;
	
	kokomi.DMGBonus -= (fanfare) * 0.23/100;
	yelan.DMGBonus -= (fanfare) * 0.23/100;
	kazuha.DMGBonus -= (fanfare) * 0.23/100;
	furina.DMGBonus -= (fanfare) * 0.23/100;
	
	kokomi.incomingHB -= (fanfare) * 0.09/100;
	yelan.incomingHB -= (fanfare) * 0.09/100;
	kazuha.incomingHB -= (fanfare) * 0.09/100;
	furina.incomingHB -= (fanfare) * 0.09/100;
	
	fanfare = 0;
	prevFanfare = 0;
	
	//22.75 furina skill
	activeChar = 4;
	
	//23.12 furina burst (cast)
	
	//23.43 kokomi ohc pop
	kokomiOHC();
	
	//23.73 furina seahorse
	furinaSeahorse();
	
	//23.93 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	//24.23 furina octopus
	furinaOctopus();
	
	//24.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//24.63 furina burst
	furina.damage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus) * hydroRes * enemyDef;
	furinaBurstActive = true;
	prevFanfare = 0;
	fanfare = 0;
	
	//25.39 furina seahorse
	furinaSeahorse();
	
	//25.58 furina crab
	furinaCrab();
	
	//25.78 yelan skill
	activeChar = 2;
	yelan.damage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (38.4/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus) * hydroRes * enemyDef;
	
	//25.93 yelan burst (cast)
	
	//26.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//26.88 furina seahorse
	furinaSeahorse();
	
	//27.20 yelan burst
	yelan.damage += (13.15/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	totalDamage += (13.15/100 * yelan.HP) * (1 + yelan.critRate * yelan.critDMG) * (1 + yelan.DMGBonus + yelanBurstDMGBonus) * hydroRes * enemyDef;
	
	yelanDMGBuff = 1.0/100;
	yelan.DMGBonus += yelanDMGBuff;
	
	//27.52 yelan n1 (cast)
	
	//27.53 kokomi ohc pop
	kokomiOHC();
	
	//25.79 furina octopus
	furinaOctopus();
	
	//27.93 yelan dice
	yelanDice();
	
	//28.03 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	//28.17 kazuha skill
	activeChar = 3;
	yelan.DMGBonus -= yelanDMGBuff;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//28.20 yelan buff iteration
	kazuha.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//28.43 furina seahorse
	furinaSeahorse();
	
	//28.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//29.07 kazuha plunge
	kazuhaPlunge();
	
	//29.20 yelan buff iteration
	kazuha.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//29.38 kazuha n1 (cast)
	
	//29.78 kazuha burst (cast)
	
	//29.80 yelan dice
	yelanDice();
	
	//30.01 furina seahorse
	furinaSeahorse();
	
	//30.20 yelan buff iteration
	kazuha.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//30.52 kokomi jellyfish
	kokomiJellyfishUnbuffed();
	
	//30.64 furina crab
	furinaCrab();
	
	//30.84 furina octopus
	furinaOctopus();
	
	//31.15 kazuha burst
	kazuha.damage += (446.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (446.0/100 * kazuha.ATK) * (1 + kazuha.critRate * kazuha.critDMG) * (1 + kazuha.DMGBonus) * enemyRes * enemyDef;
	
	kazuha.damage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
	totalDamage += (0.6 * 1446.85 * (1 + (16 * kazuha.EM)/(2000/1.0 + kazuha.EM) + 60.0/100)) * hydroRes;
	
	//31.20 yelan buff iteration
	kazuha.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kazuha.DMGBonus += yelanDMGBuff;
	
	//31.53 kokomi ohc pop
	kokomiOHC();
	
	//31.54 furina seahorse
	furinaSeahorse();
	
	//31.58 kokomi n1 (cast)
	activeChar = 1;
	kazuha.DMGBonus -= yelanDMGBuff;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//31.73 kokomi burst (cast)
	
	//31.82 kokomi n1 (unbuffed)
	kokomi.damage += (123.1/100 * kokomi.ATK + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (123.1/100 * kokomi.ATK + 1.0/100 * kokomi.HP * kokomiUsingMoonglow) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
//	kokomiBotBBStacks++;
	
	//32.00 yelan dice
	yelanDice();
	
	//32.03 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	//32.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//32.17 kazuha dot
	kazuhaDot();
	
	//32.52 kokomi jellyfish
	kokomiJellyfish();
	
	//32.55 kokomi burst
	kokomi.damage += (18.7/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	totalDamage += (18.7/100 * kokomi.HP) * (1 + kokomi.DMGBonus) * hydroRes * enemyDef;
	
	//33.09 furina seahorse
	furinaSeahorse();
	
	//33.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//33.30 kokomi n1
	kokomiN1();
//	kokomiBotBBStacks++;

	//33.48 yelan dice
	yelanDice();
	
	//33.67 kokomi n2
	kokomiN2();
//	kokomiBotBBStacks++;
	
	//34.09 furina octopus
	furinaOctopus();
	
	//34.12 kazuha dot
	kazuhaDot();
	
	//34.15 kokomi n1
	kokomiN1();
	
	//34.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//34.48 yelan dice
	yelanDice();
	
	//34.52 kokomi jellyfish
	kokomiJellyfish();
	
	//34.52 kokomi n2
	kokomiN2();
	
	//34.64 furina seahorse
	furinaSeahorse();
	
//	//13.25 kokomi prototype amber burst heal
//	kokomi.currHP += 6.0/100;
//	yelan.currHP += 6.0/100;
//	kazuha.currHP += 6.0/100;
//	furina.currHP += 6.0/100;
//	
//	if(kokomi.currHP > 1) kokomi.currHP = 1;
//	if(yelan.currHP > 1) yelan.currHP = 1;
//	if(kazuha.currHP > 1) kazuha.currHP = 1;
//	if(furina.currHP > 1) furina.currHP = 1;
	
	//35.00 kokomi n1
	kokomiN1();
	
	//35.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//35.37 kokomi n2
	kokomiN2();
	
//	//35.42 kokomi (flowing purity r5) initial dmg bonus buff expires
//	kokomi.DMGBonus -= 16.0/100;
	
	//35.48 yelan dice
	yelanDice();
	
	//35.53 kokomi ohc pop
	kokomiOHC();
	
	//35.70 furina crab
	furinaCrab();
	
	//35.85 kokomi n1
	kokomiN1();
	
	//36.03 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	//36.07 kazuha dot
	kazuhaDot();
	
	//36.19 furina seahorse
	furinaSeahorse();
	
	//36.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//36.22 kokomi n2
	kokomiN2();
	
	//36.52 kokomi jellyfish
	kokomiJellyfish();
	
//	//15.25 kokomi prototype amber burst heal
//	kokomi.currHP += 6.0/100;
//	yelan.currHP += 6.0/100;
//	kazuha.currHP += 6.0/100;
//	furina.currHP += 6.0/100;
//	
//	if(kokomi.currHP > 1) kokomi.currHP = 1;
//	if(yelan.currHP > 1) yelan.currHP = 1;
//	if(kazuha.currHP > 1) kazuha.currHP = 1;
//	if(furina.currHP > 1) furina.currHP = 1;
	
	//36.70 kokomi n1
	kokomiN1();
	
	//36.88 yelan dice
	yelanDice();
	
	//37.07 kokomi n2
	kokomiN2();
	
	//37.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//37.34 furina octopus
	furinaOctopus();
	
	//37.55 kokomi n1
	kokomiN1();
	
	//37.74 furina seahorse
	furinaSeahorse();
	
	//37.88 yelan dice
	yelanDice();
	
	//37.92 kokomi n2
	kokomiN2();
	
	//38.02 kazuha dot
	kazuhaDot();
	
	//38.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//38.38 kokomi n1
	kokomiN1();
	
	//38.52 kokomi jellyfish
	kokomiJellyfish();
	
//	//17.25 kokomi prototype amber burst heal
//	kokomi.currHP += 6.0/100;
//	yelan.currHP += 6.0/100;
//	kazuha.currHP += 6.0/100;
//	furina.currHP += 6.0/100;
//	
//	if(kokomi.currHP > 1) kokomi.currHP = 1;
//	if(yelan.currHP > 1) yelan.currHP = 1;
//	if(kazuha.currHP > 1) kazuha.currHP = 1;
//	if(furina.currHP > 1) furina.currHP = 1;
	
	//38.75 kokomi n2
	kokomiN2();
	
	//38.88 yelan dice
	yelanDice();
	
	//39.15 vv uptime wears off
	hydroRes = 0.9;
	
	//39.20 yelan buff iteration
	kokomi.DMGBonus -= yelanDMGBuff;
	yelanDMGBuff += 3.5/100;
	kokomi.DMGBonus += yelanDMGBuff;
	
	//39.22 kokomi n1
	kokomiN1();
	
	//39.23 kokomi ohc pop
	kokomiOHC();
	
	//39.29 furina seahorse
	furinaSeahorse();
	
	//39.58 kokomi n2
	kokomiN2();
	
	//39.73 kokomi ohc reset
	kokomiOHCHeal = 0.0;
	
	
	
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
