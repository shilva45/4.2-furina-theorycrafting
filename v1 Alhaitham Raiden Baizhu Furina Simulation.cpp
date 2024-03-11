#include <stdio.h>
#include <string.h>

double prevTime = 0.0;
double time = 0.0;

double dendroRes = 0.9; // dendro
double enemyRes = 0.9; // electro and hydro
double enemyDef = 0.5;

double totalDamage = 0.0;
double prevFanfare = 0.0;
double fanfare = 0.0;

double alhaithamSpread = 0.0;

int activeChar = 2;

double baizhuOHCHeal = 0.0;
double baizhuBloomBuff = 0.0;
double baizhuAdditiveBuff = 0.0;
bool furinaBurstActive = false;
int alhaithamLofiActive = 0;
int alhaithamLofiStacks = 0;
			 
int blooms;
int hyperblooms = 0;
int aggravates;
int spreads;

struct {
	char name[10];
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
} alhaitham, raiden, baizhu, furina;

void insertData(){
	strcpy(alhaitham.name, "Alhaitham"); // 1.4k atk, 436 em | lofi, 4p gilded, EM/Dendro/Crit, 9/9/9
	alhaitham.HP = 13348 * (1 + 0.0992) + 4780 + 507.88; // base + substat
	alhaitham.ATK = (313 + 542) * (1 + 0.2976) + 311 + 33.08; // base + sig + substat + substat + substat
	alhaitham.ER = 1 + 0.1102;
	alhaitham.EM = 187 + 118.92 + 50 + 80; // em sands + substat + dendro resonance + 2p gilded
	alhaitham.DMGBonus = 46.6/100.0 + 28.8/100; // dendro gob + ascension
	alhaitham.critRate = 5/100.0 + 31.1/100.0 + 0.331 + 4.0/100; // base + cr circlet + substat + lofi
	alhaitham.critDMG = 50/100.0 + 88.0/100 + 0.3972; // base + lofi + substat
	alhaitham.currHP = 100.0/100.0;
	alhaitham.HB = 0.0/100.0;
	
	strcpy(raiden.name, "Raiden"); // 895 em | dragons bane, 4p dwm, EM/EM/Crit, 1/9/9
	raiden.HP = 12907 * (1 + 0.0992/100) + 4780 + 507.88; // base + substat
	raiden.ATK = (337 + 454) * (1 + 0.0992/100.0) + 311 + 33.08; // base + sig + substat
	raiden.ER = 1 + 0.1102;
	raiden.EM = 165 + 187 + 187 + 187 + 50 + 118.92; // dragons bane + em sands + em gob + dendro resonance + substat
	raiden.DMGBonus = 0/100.0;
	raiden.critRate = 5/100.0 + 0.0662; // base + substat
	raiden.critDMG = 50/100.0 + 0.1324; // base + substat
	raiden.currHP = 100.0/100.0;
	raiden.HB = 0.0/100.0;
	
	strcpy(baizhu.name, "Baizhu"); // 46k hp | prototype amber, 4p ohc, HP/HP/HB, 1/9/9
	baizhu.HP = 13380 * (1 + (28.8 + 46.6 + 46.6 + 41.3 + 0.2976)/100) + 4780 + 507.88; // base + ascension + hp sands + hp gob + proto amber + substat
	baizhu.ATK = (193 + 510) * (1 + 0.0992) + 311 + 33.08; // base + fav codex + substat
	baizhu.ER = 1 + 0.3857;
	baizhu.EM = 39.64 + 50; // substat + dendro resonance
	baizhu.DMGBonus = 25/100.0; // a1 passive
	baizhu.critRate = 5/100.0 + 0.2648; // base + substat
	baizhu.critDMG = 50/100.0 + 0.4634; // base + substat
	baizhu.currHP = 100.0/100.0;
	baizhu.HB = 15.0/100 + 31.9/100; // 2p ohc + hb circlet
	baizhu.incomingHB = 0.0/100.0;
	
	strcpy(furina.name, "Furina"); // 39k hp | fav sword, 4p gt, HP/HP/Crit, 1/9/9
	furina.HP = 15307 * (1 + 46.6/100 + 46.6/100 + 0.2976) + 4780 + 507.88; // base + hp sands + hp gob + substat
	furina.ER = 1 + 0.2755 + 61.3/100;
	furina.EM = 39.64 + 50; // substat + dendro resonance
	furina.DMGBonus = (0.7/100 * furina.HP/1000) < 28.0/100 ? (0.7/100 * furina.HP/1000) : 28.0/100; // a4 passive
	furina.DMGBonus += 70/100.0; // 4p gt
	furina.critRate = 5/100.0 + 19.2/100 + 0.2648; // base + ascension + substat
	furina.critDMG = 50/100.0 + 62.2/100 + 0.5958; // base + cd circlet + substat
	furina.currHP = 100.0/100.0;
	furina.HB = 0.0/100.0;
	furina.incomingHB = 0.0/100.0;
}

void viewData(){
	printf("%s HP: %.0f\n", alhaitham.name, alhaitham.HP);
	printf("%s ATK: %.0f\n", alhaitham.name, alhaitham.ATK);
	printf("%s EM: %.0f\n", alhaitham.name, alhaitham.EM);
	printf("%s DMG Bonus: %.1f%%\n", alhaitham.name, alhaitham.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", alhaitham.name, alhaitham.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", alhaitham.name, alhaitham.critDMG * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", alhaitham.name, alhaitham.incomingHB * 100);
	
	puts("");
	
	printf("%s HP: %.0f\n", raiden.name, raiden.HP);
	printf("%s ATK: %.0f\n", raiden.name, raiden.ATK);
	printf("%s EM: %.0f\n", raiden.name, raiden.EM);
	printf("%s DMG Bonus: %.1f%%\n", raiden.name, raiden.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", raiden.name, raiden.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", raiden.name, raiden.critDMG * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", raiden.name, raiden.incomingHB * 100);
	
	puts("");
	
	printf("%s HP: %.0f\n", baizhu.name, baizhu.HP);
	printf("%s ATK: %.0f\n", baizhu.name, baizhu.ATK);
	printf("%s EM: %.0f\n", baizhu.name, baizhu.EM);
	printf("%s DMG Bonus: %.1f%%\n", baizhu.name, baizhu.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", baizhu.name, baizhu.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", baizhu.name, baizhu.critDMG * 100);
	printf("%s Healing Bonus: %.1f%%\n", baizhu.name, baizhu.HB * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", baizhu.name, baizhu.incomingHB * 100);
	
	puts("");
	
	printf("%s HP: %.0f\n", furina.name, furina.HP);
	printf("%s EM: %.0f\n", furina.name, furina.EM);
	printf("%s DMG Bonus: %.1f%%\n", furina.name, furina.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", furina.name, furina.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", furina.name, furina.critDMG * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", furina.name, furina.incomingHB * 100);
	
	puts("");
}

void printCurrHP(){
	printf("Time: %.2f\n", time);
	printf("Fanfare: %.1f\n", fanfare);	
	puts("");
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", alhaitham.name, alhaitham.currHP * 100, alhaitham.currHP * alhaitham.HP, alhaitham.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", raiden.name, raiden.currHP * 100, raiden.currHP * raiden.HP, raiden.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", baizhu.name, baizhu.currHP * 100, baizhu.currHP * baizhu.HP, baizhu.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", furina.name, furina.currHP * 100, furina.currHP * furina.HP, furina.HP);
	
	puts("");
	puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	puts("");
}

void furinaSeahorse(){
	furina.damage += (5.49/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	totalDamage += (5.49/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	alhaitham.prevHP = alhaitham.currHP;
	raiden.prevHP = raiden.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	alhaitham.currHP -= 1.6/100;
	raiden.currHP -= 1.6/100;
	baizhu.currHP -= 1.6/100;
	furina.currHP -= 1.6/100;
	
	if(!furinaBurstActive){
		printCurrHP();
		return;
	}
	
	prevFanfare = fanfare;
	fanfare += 4 * 1.6;
	
	if(fanfare > 300) fanfare = 300;
	
	alhaitham.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	raiden.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	alhaitham.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	raiden.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	
	printCurrHP();
}

void furinaOctopus(){
	furina.damage += (10.13/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	totalDamage += (10.13/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	alhaitham.prevHP = alhaitham.currHP;
	raiden.prevHP = raiden.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	alhaitham.currHP -= 2.4/100;
	raiden.currHP -= 2.4/100;
	baizhu.currHP -= 2.4/100;
	furina.currHP -= 2.4/100;
	
	if(!furinaBurstActive){
		printCurrHP();
		return;
	}
	
	prevFanfare = fanfare;
	fanfare += 4 * 2.4;
	
	if(fanfare > 300) fanfare = 300;
	
	alhaitham.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	raiden.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	alhaitham.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	raiden.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	
	printCurrHP();
}

void furinaCrab(){
	furina.damage += (14.09/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	totalDamage += (14.09/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	alhaitham.prevHP = alhaitham.currHP;
	raiden.prevHP = raiden.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	alhaitham.currHP -= 3.6/100;
	raiden.currHP -= 3.6/100;
	baizhu.currHP -= 3.6/100;
	furina.currHP -= 3.6/100;
	
	if(!furinaBurstActive){
		printCurrHP();
		return;
	}
	
	prevFanfare = fanfare;
	fanfare += 4 * 3.6;
	
	if(fanfare > 300) fanfare = 300;
	
	alhaitham.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	raiden.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	alhaitham.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	raiden.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	
	printCurrHP();
}

void raidenSkill(){
	raiden.damage += (199.2/100 * raiden.ATK) * (1 + raiden.critRate * raiden.critDMG) * (1 + raiden.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (199.2/100 * raiden.ATK) * (1 + raiden.critRate * raiden.critDMG) * (1 + raiden.DMGBonus) * enemyRes * enemyDef;
	
	printf("Blooms present: %d\n", blooms);
	if(blooms > 2) blooms = 2;
	
	for(int i = 1; i <= blooms; i++){
		raiden.damage += 3 * 1446.85 * (1 + (16.0 * raiden.EM)/(2000.0 + raiden.EM)) * dendroRes;
		totalDamage += 3 * 1446.85 * (1 + (16.0 * raiden.EM)/(2000.0 + raiden.EM)) * dendroRes;
		hyperblooms++;
	}
	
	blooms = 0;
	
	printCurrHP();
}

void alhaithamN1(double spread){
	alhaitham.damage += (91.0/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (91.0/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	
	printf("This attack dealt %.1f damage.\n\n", (91.0/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	alhaithamLofiStacks--;
	
	if(alhaithamLofiStacks <= 0){
		alhaithamLofiActive = 0;
		alhaithamLofiStacks = 0;
	}
	
	printf("Lofi Stacks: %d\n", alhaithamLofiStacks);
	
	printCurrHP();
}

void alhaithamN2(double spread){
	alhaitham.damage += (93.2/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (93.2/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;

	printf("This attack dealt %.1f damage.\n\n", (93.2/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	alhaithamLofiStacks--;
	
	if(alhaithamLofiStacks <= 0){
		alhaithamLofiActive = 0;
		alhaithamLofiStacks = 0;
	}
	
	printf("Lofi Stacks: %d\n", alhaithamLofiStacks);
	
	printCurrHP();
}

void alhaithamN3(double spread){
	alhaitham.damage += (62.8/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (62.8/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;

	printf("This attack dealt %.1f damage.\n\n", (62.8/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	alhaithamLofiStacks--;
	
	if(alhaithamLofiStacks <= 0){
		alhaithamLofiActive = 0;
		alhaithamLofiStacks = 0;
	}
	
	printf("Lofi Stacks: %d\n", alhaithamLofiStacks);
	
	printCurrHP();
}

void alhaithamN4(double spread){
	alhaitham.damage += (122.7/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (122.7/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;

	printf("This attack dealt %.1f damage.\n\n", (122.7/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	alhaithamLofiStacks--;
	
	if(alhaithamLofiStacks <= 0){
		alhaithamLofiActive = 0;
		alhaithamLofiStacks = 0;
	}
	
	printf("Lofi Stacks: %d\n", alhaithamLofiStacks);
	
	printCurrHP();
}

void alhaithamN5(double spread){
	alhaitham.damage += (154.1/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (154.1/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;

	printf("This attack dealt %.1f damage.\n\n", (154.1/100 * alhaitham.ATK + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	alhaithamLofiStacks--;
	
	if(alhaithamLofiStacks <= 0){
		alhaithamLofiActive = 0;
		alhaithamLofiStacks = 0;
	}
	
	printf("Lofi Stacks: %d\n", alhaithamLofiStacks);
	
	printCurrHP();
}

void alhaithamCharge(double spread){
	alhaitham.damage += (101.5/100 * alhaitham.ATK + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (101.5/100 * alhaitham.ATK + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;

	printf("This attack dealt %.1f damage.\n\n", (101.5/100 * alhaitham.ATK + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	printCurrHP();
}

void alhaithamPlunge(double spread){
	alhaitham.damage += (235.0/100 * alhaitham.ATK + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (235.0/100 * alhaitham.ATK + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	
	printf("This attack dealt %.1f damage.\n\n", (235.0/100 * alhaitham.ATK + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	printCurrHP();
}

void alhaithamSkill(double spread){
	alhaitham.damage += ((329.1/100 * alhaitham.ATK + 263.3/100 * alhaitham.EM) + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;
	totalDamage += ((329.1/100 * alhaitham.ATK + 263.3/100 * alhaitham.EM) + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef;

	printf("This attack dealt %.1f damage.\n\n", ((329.1/100 * alhaitham.ATK + 263.3/100 * alhaitham.EM) + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	alhaithamLofiStacks--;
	
	if(alhaithamLofiStacks <= 0){
		alhaithamLofiActive = 0;
		alhaithamLofiStacks = 0;
	}
	
	printCurrHP();
}

void alhaithamMirror(double spread){
	alhaitham.damage += ((114.2/100 * alhaitham.ATK + 228.5/100 * alhaitham.EM) + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus + 0.1/100 * alhaitham.EM) * dendroRes * enemyDef;
	totalDamage += ((114.2/100 * alhaitham.ATK + 228.5/100 * alhaitham.EM) + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus + 0.1/100 * alhaitham.EM) * dendroRes * enemyDef;

	printf("This attack dealt %.1f damage.\n\n", ((114.2/100 * alhaitham.ATK + 228.5/100 * alhaitham.EM) + spread + alhaithamLofiActive * 120.0/100 * alhaitham.EM) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	alhaithamLofiStacks--;
	
	if(alhaithamLofiStacks <= 0){
		alhaithamLofiActive = 0;
		alhaithamLofiStacks = 0;
	}
	
	printf("Lofi Stacks: %d\n", alhaithamLofiStacks);
	
	printCurrHP();
}

void alhaithamBurst(double spread){
	alhaitham.damage += ((206.7/100 * alhaitham.ATK + 165.4/100 * alhaitham.EM) + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus + 0.1/100 * alhaitham.EM) * dendroRes * enemyDef;
	totalDamage += ((206.7/100 * alhaitham.ATK + 165.4/100 * alhaitham.EM) + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus + 0.1/100 * alhaitham.EM) * dendroRes * enemyDef;
	
	printf("This attack dealt %.1f damage.\n\n", ((206.7/100 * alhaitham.ATK + 165.4/100 * alhaitham.EM) + spread) * (1 + alhaitham.critDMG * alhaitham.critRate) * (1 + alhaitham.DMGBonus) * dendroRes * enemyDef);
	
	printCurrHP();
}

void baizhuOHC(){
	baizhuOHCHeal > 30000 ? baizhuOHCHeal = 30000 : baizhuOHCHeal = baizhuOHCHeal;
	baizhu.damage += baizhuOHCHeal * 0.9 * enemyRes;
	totalDamage += baizhuOHCHeal * 0.9 * enemyRes;
}

void baizhuSkillHeal(){
	alhaitham.prevHP = alhaitham.currHP;
	raiden.prevHP = raiden.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	alhaitham.currHP += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + alhaitham.incomingHB) / alhaitham.HP;
	raiden.currHP += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + raiden.incomingHB) / raiden.HP;
	baizhu.currHP += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + baizhu.incomingHB) / baizhu.HP;
	furina.currHP += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + furina.incomingHB) / furina.HP;
	baizhuOHCHeal += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + alhaitham.incomingHB);
	baizhuOHCHeal += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + raiden.incomingHB);
	baizhuOHCHeal += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + baizhu.incomingHB);
	baizhuOHCHeal += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + furina.incomingHB);
	if(alhaitham.currHP > 1) alhaitham.currHP = 1;
	if(raiden.currHP > 1) raiden.currHP = 1;
	if(baizhu.currHP > 1) baizhu.currHP = 1;
	if(furina.currHP > 1) furina.currHP = 1;
	prevFanfare = fanfare;
	fanfare += (alhaitham.currHP - alhaitham.prevHP) * 100;
	fanfare += (raiden.currHP - raiden.prevHP) * 100;
	fanfare += (baizhu.currHP - baizhu.prevHP) * 100;
	fanfare += (furina.currHP - furina.prevHP) * 100;
	
	alhaitham.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	raiden.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	alhaitham.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	raiden.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	
	printCurrHP();
}

void baizhuBurstHeal(){
	alhaitham.prevHP = alhaitham.currHP;
	raiden.prevHP = raiden.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	
	switch (activeChar){
		case 1:
			alhaitham.currHP += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + alhaitham.incomingHB) / alhaitham.HP;
			baizhuOHCHeal += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + alhaitham.incomingHB);
			if(alhaitham.currHP > 1) alhaitham.currHP = 1;
			break;
		case 2:
			raiden.currHP += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + raiden.incomingHB) / raiden.HP;
			baizhuOHCHeal += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + raiden.incomingHB);
			if(raiden.currHP > 1) raiden.currHP = 1;
			break;
		case 3:
			baizhu.currHP += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + baizhu.incomingHB) / baizhu.HP;
			baizhuOHCHeal += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + baizhu.incomingHB);
			if(baizhu.currHP > 1) baizhu.currHP = 1;
			break;
		case 4:
			furina.currHP += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + furina.incomingHB) / furina.HP;
			baizhuOHCHeal += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + furina.incomingHB);
			if(furina.currHP > 1) furina.currHP = 1;
			break;
		default:
			break;
	}
	
	baizhuBloomBuff = (baizhu.HP/1000 * 2.0/100) < 100.0/100 ? (baizhu.HP/1000 * 2.0/100) : 100.0/100;
	baizhuAdditiveBuff = (baizhu.HP/1000 * 0.8/100) < 40.0/100 ? (baizhu.HP/1000 * 0.8/100) : 40.0/100;
	
	if(!furinaBurstActive){
		printCurrHP();
		return;
	}
	
	prevFanfare = fanfare;
	fanfare += 100 * (alhaitham.currHP - alhaitham.prevHP);
	fanfare += 100 * (raiden.currHP - raiden.prevHP);
	fanfare += 100 * (baizhu.currHP - baizhu.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	alhaitham.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	raiden.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	alhaitham.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	raiden.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	
	printCurrHP();
}

void baizhuProtoHeal(){
	alhaitham.prevHP = alhaitham.currHP;
	raiden.prevHP = raiden.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	
	alhaitham.currHP += 6.0/100 * (1 + alhaitham.incomingHB);
	if(alhaitham.currHP > 1) alhaitham.currHP = 1;
	raiden.currHP += 6.0/100 * (1 + raiden.incomingHB);
	if(raiden.currHP > 1) raiden.currHP = 1;
	baizhu.currHP += 6.0/100 * (1 + baizhu.incomingHB);
	if(baizhu.currHP > 1) baizhu.currHP = 1;
	furina.currHP += 6.0/100 * (1 + furina.incomingHB);
	if(furina.currHP > 1) furina.currHP = 1;
	
	if(!furinaBurstActive){
		printCurrHP();
		return;
	}
	
	prevFanfare = fanfare;
	fanfare += 100 * (alhaitham.currHP - alhaitham.prevHP);
	fanfare += 100 * (raiden.currHP - raiden.prevHP);
	fanfare += 100 * (baizhu.currHP - baizhu.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	alhaitham.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	raiden.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	alhaitham.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	raiden.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	
	baizhuBloomBuff = baizhu.HP/1000 * 2.0/100;
	baizhuAdditiveBuff = baizhu.HP/1000 * 0.8/100;
	
	printCurrHP();
}

void baizhuBurstDamage(){
	baizhu.damage += (165.0/100 * baizhu.ATK) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (165.0/100 * baizhu.ATK) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	
	printCurrHP();
}




int auraType = 8;
double auraGauge = 0.0;
double remainingGauge = 0.0;
double auraGaugeElectro = 0.0;
double auraGaugeDendro = 0.0;
double auraGaugeHydro = 0.0;
double auraGaugeQuicken = 0.0;
double newAuraGaugeQuicken = 0.0;
double decayRate = 0.0;
double decayRateQuicken = 0.0;
char elements[15][35] = {"-", "Anemo", "Geo", "Electro", "Dendro", "Hydro", "Pyro", "Cryo", "No", 
						 "Quicken", "Quicken (Underlying Electro)", "Quicken (Underlying Dendro)", "Electro-Charged"};
						 

void electroChargedTick(){
	auraGauge -= 0.4;
	auraGaugeHydro -= 0.4;
	auraGaugeElectro -= 0.4;
	
	printf("- Electro-Charged triggered at %.2fs. ", time);
	if(auraGaugeHydro > 0) printf("%.3fU of Hydro remain. ", auraGaugeHydro);
	else printf("Hydro expired. ");
	
	if(auraGaugeHydro > 0 && auraGaugeElectro > 0) printf("and ");
	
	if(auraGaugeElectro > 0) printf("%.3fU of Electro remain. ", auraGaugeElectro);
	else printf("Electro expired. ");
	
	puts("");
	
	if(auraGaugeHydro > 0 && auraGaugeElectro <= 0){
		auraType = 5;
		auraGauge = auraGaugeHydro;
	} else if(auraGaugeHydro <= 0 && auraGaugeElectro > 0){
		auraType = 3;
		auraGauge = auraGaugeElectro;
	} else if(auraGaugeHydro <= 0 && auraGaugeElectro <= 0){
		auraType = 8;
		auraGauge = 0.0;
	}
}

void reaction(int applyElement, double applyGauge){
	if(auraType != 10 && auraType != 11 && auraType != 12){
		auraGauge -= (time - prevTime) * decayRate;
		if(auraGauge <= 0){
			auraType = 8;
			auraGauge = 0.0;
		}
		
		printf("%s Aura of %.3fU remain at %.2fs. Applying %.3fU of %s.\n", elements[auraType], auraGauge, time, applyGauge, elements[applyElement]);
	} else if(auraType == 9){
		auraGaugeQuicken -= (time - prevTime) * decayRateQuicken;
		
		printf("%s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
		printf("remain at %.2fs. Applying %.3fU of %s.\n", time, applyGauge, elements[applyElement]);
	} else if(auraType == 10){
		// Quicken with underlying Electro
		auraGaugeQuicken -= (time - prevTime) * decayRateQuicken;
		auraGaugeElectro -= (time - prevTime) * decayRateQuicken;
		
		if(auraGaugeElectro < 0){
			printf("Underlying Electro expired. ");
			auraType = 9;
		}
		
		printf("%s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
		if(auraGaugeElectro > 0) printf("and Electro Aura of %.3fU ", auraGaugeElectro);
		printf("remain at %.2fs. Applying %.3fU of %s.\n", time, applyGauge, elements[applyElement]);
	} else if(auraType == 11){
		// Quicken with underlying Dendro
		auraGaugeQuicken -= (time - prevTime) * decayRateQuicken;
		auraGaugeDendro -= (time - prevTime) * decayRateQuicken;
		
		if(auraGaugeDendro < 0){
			printf("Underlying Dendro expired. ");
			auraType = 9;
		}
		
		printf("%s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
		if(auraGaugeDendro > 0) printf("and Dendro Aura of %.3fU ", auraGaugeDendro);
		printf("remain at %.2fs. Applying %.3fU of %s.\n", time, applyGauge, elements[applyElement]);
	} else if(auraType == 12){
		// EC Edge case
		auraGaugeHydro -= (time - prevTime) * decayRate;
		auraGaugeElectro -= (time - prevTime) * decayRate;
		
		printf("%s Aura (%.3fU Hydro, %.3fU Electro) remain at %.2fs. Applying %.3fU of %s.\n", elements[auraType], auraGaugeHydro, auraGaugeElectro, time, applyGauge, elements[applyElement]);
	}
	
	
	
	switch (auraType){
		// Electro Aura
		case 3:
			switch (applyElement){
				// Electro Aura + Electro
				case 3:
					auraType = 3;
					applyGauge *= 0.8;
					auraGauge = auraGauge > applyGauge ? auraGauge : applyGauge;
					
					printf("%s Aura of %.3fU refreshed at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
				// Electro Aura + Dendro = Quicken
				case 4:
					auraType = 9;
					auraGaugeQuicken = auraGauge < applyGauge ? auraGauge : applyGauge;
					decayRateQuicken = 1 / (auraGauge * 5 + 6);
					
					printf("%s activated. Quicken Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
					if(applyGauge < auraGauge){
						auraType = 10;
						auraGaugeElectro = auraGauge - applyGauge;
						decayRate = 1/(2.5 * auraGaugeElectro + 7);
//						decayRate = 1/(2.5 * applyGauge + 7);
//						decayRate = 1/((35 / (4 * auraGaugeElectro)) + (25 / 8));
						printf("and Underlying Electro Aura of %.3fU ", auraGaugeElectro);
					}
					printf("applied at %.2fs\n", auraGauge, time);
					
					
					break;
					
				// Electro Aura + Hydro = Electro-Charged Aura
				case 5:
					auraType = 12;
					auraGaugeHydro = applyGauge * 0.8;
					auraGaugeElectro = auraGauge;
					auraGauge = auraGaugeHydro > auraGaugeElectro ? auraGaugeHydro : auraGaugeElectro;
					
					electroChargedTick();
					
					if(auraGaugeHydro > 0 && auraGaugeElectro > 0){
						printf("%s triggered. Hydro Aura of %.3fU and Electro Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGaugeHydro, auraGaugeElectro, time);
					} else if(auraGaugeHydro > 0 && auraGaugeElectro <= 0){
						auraType = 5;
						auraGauge = auraGaugeHydro;
						printf("%s Aura of %.3fU refreshed at %.2fs\n", elements[auraType], auraGauge, time);
					} else if(auraGaugeHydro <= 0 && auraGaugeElectro > 0){
						auraType = 3;
						auraGauge = auraGaugeElectro;
						printf("%s Aura of %.3fU refreshed at %.2fs\n", elements[auraType], auraGauge, time);
					} else {
						auraType = 8;
						auraGauge = 0.0;
						printf("%s Aura remains at %.2fs\n", elements[auraType], auraGauge, time);
					}
					
					break;
				
				default:
					break;
			}
		
			break;
		
		
		
		
		
		// Dendro Aura
		case 4:
			switch (applyElement){
				// Dendro Aura + Electro = Quicken
				case 3:
					auraType = 9;
					auraGaugeQuicken = auraGauge < applyGauge ? auraGauge : applyGauge;
					decayRateQuicken = 1 / (auraGauge * 5 + 6);
					
					printf("%s activated. Quicken Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
					if(applyGauge < auraGauge){
						auraType = 11;
						auraGaugeDendro = auraGauge - applyGauge;
						decayRate = 1/(2.5 * auraGaugeDendro + 7);
						printf("and Underlying Dendro Aura of %.3fU ", auraGaugeElectro);
					}
					printf("applied at %.2fs\n", auraGauge, time);
					
					break;
					
				// Dendro Aura + Dendro
				case 4:
					auraType = 4;
					applyGauge *= 0.8;
					auraGauge = auraGauge > applyGauge ? auraGauge : applyGauge;
					
					printf("%s Aura of %.3fU refreshed at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
				// Dendro Aura + Hydro = Reverse Bloom
				case 5:
					auraGauge -= 0.5 * applyGauge;
					blooms++;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Bloom created. %s Aura remains at %.2fs\n", elements[auraType], auraGauge, time);
						break;
					}
					
					printf("Bloom created. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
				
				default:
					break;
			}
		
			break;
		
		
		
		
		
		
		// Hydro Aura
		case 5:
			switch (applyElement){
				// Hydro Aura + Electro = Electro-Charged Aura
				case 3:
					auraType = 12;
					auraGaugeHydro = auraGauge;
					auraGaugeElectro = applyGauge * 0.8;
					auraGauge = auraGaugeHydro > auraGaugeElectro ? auraGaugeHydro : auraGaugeElectro;
					
					electroChargedTick();
					
					if(auraGaugeHydro > 0 && auraGaugeElectro > 0){
						printf("%s triggered. Hydro Aura of %.3fU and Electro Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGaugeHydro, auraGaugeElectro, time);
					} else if(auraGaugeHydro > 0 && auraGaugeElectro <= 0){
						auraType = 5;
						auraGauge = auraGaugeHydro;
						printf("%s Aura of %.3fU refreshed at %.2fs\n", elements[auraType], auraGauge, time);
					} else if(auraGaugeHydro <= 0 && auraGaugeElectro > 0){
						auraType = 3;
						auraGauge = auraGaugeElectro;
						printf("%s Aura of %.3fU refreshed at %.2fs\n", elements[auraType], auraGauge, time);
					} else {
						auraType = 8;
						auraGauge = 0.0;
						printf("%s Aura remains at %.2fs\n", elements[auraType], auraGauge, time);
					}
					
					break;
					
				// Hydro Aura + Dendro = Forward Bloom
				case 4:
					auraGauge -= 2.0 * applyGauge;
					blooms++;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Bloom created. %s Aura remains at %.2fs\n", elements[auraType], time);
						break;
					}
					
					printf("Bloom created. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
				// Hydro Aura + Hydro
				case 5:
					auraType = 5;
					applyGauge *= 0.8;
					auraGauge = auraGauge > applyGauge ? auraGauge : applyGauge;
					
					printf("%s Aura of %.3fU refreshed at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
				
				default:
					break;
			}
		
			break;
		
		
		
		
		
		// No Aura
		case 8:
			switch (applyElement){
				// No Aura + Electro
				case 3:
					auraType = 3;
					auraGauge = 0.8 * applyGauge;
					if(applyGauge == 1.0) decayRate = 1/11.875;
					else if(applyGauge == 1.5) decayRate = 1/8.9583;
					else if(applyGauge == 2.0) decayRate = 1/7.5;
					else if(applyGauge == 4.0) decayRate = 1/5.3125;
					
					printf("%s Aura of %.3fU applied at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
				// No Aura + Dendro
				case 4:
					auraType = 4;
					auraGauge = 0.8 * applyGauge;
					if(applyGauge == 1.0) decayRate = 1/11.875;
					else if(applyGauge == 1.5) decayRate = 1/8.9583;
					else if(applyGauge == 2.0) decayRate = 1/7.5;
					else if(applyGauge == 4.0) decayRate = 1/5.3125;
					
					printf("%s Aura of %.3fU applied at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
				// No Aura + Hydro
				case 5:
					auraType = 5;
					auraGauge = 0.8 * applyGauge;
					if(applyGauge == 1.0) decayRate = 1/11.875;
					else if(applyGauge == 1.5) decayRate = 1/8.9583;
					else if(applyGauge == 2.0) decayRate = 1/7.5;
					else if(applyGauge == 4.0) decayRate = 1/5.3125;
					
					printf("%s Aura of %.3fU applied at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
					
				
				default:
					break;
			}
		
			break;
			
			
		
		// Quicken Aura
		case 9:
			switch (applyElement){
				// Quicken Aura + Electro = Aggravate
				case 3:
					if(applyGauge > 0){
						auraType = 10;
						auraGaugeElectro = applyGauge * 0.8;
						if(auraGaugeElectro == 1.0) decayRate = 1/11.875;
						else if(auraGaugeElectro == 1.5) decayRate = 1/8.9583;
						else if(auraGaugeElectro == 2.0) decayRate = 1/7.5;
						else if(auraGaugeElectro == 4.0) decayRate = 1/5.3125;
					}
					aggravates++;
					printf("Aggravate triggered. %s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
					if(auraGaugeElectro > 0) printf("and Underlying Electro Aura of %.3fU ", auraGaugeElectro);
					printf("remain at %.2fs\n", time);
					
					break;
					
				// Quicken Aura + Dendro = Spread
				case 4:
					if(applyGauge > 0){
						auraType = 11;
						auraGaugeDendro = applyGauge * 0.8;
						if(auraGaugeDendro == 1.0) decayRate = 1/11.875;
						else if(auraGaugeDendro == 1.5) decayRate = 1/8.9583;
						else if(auraGaugeDendro == 2.0) decayRate = 1/7.5;
						else if(auraGaugeDendro == 4.0) decayRate = 1/5.3125;
//						decayRate = 1/(2.5 * auraGaugeDendro + 7);
					}
					spreads++;
					printf("Spread triggered. %s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
					if(auraGaugeDendro > 0) printf("and Underlying Dendro Aura of %.3fU ", auraGaugeDendro);
					printf("remain at %.2fs\n", time);
					
					break;
					
				// Quicken Aura + Hydro = Reverse Bloom
				case 5:
					auraGaugeHydro = applyGauge - 2.0 * auraGaugeQuicken;
					auraGaugeQuicken -= 0.5 * applyGauge;
					blooms++;
					if(auraGaugeQuicken <= 0){
						printf("Bloom created. Quicken expired. ");
						printf("Hydro Aura of %.3fU remain at %.2fs\n", auraGaugeHydro, time);
						
						auraType = 8;
						auraGauge = 0.0;
						break;
					}
					
					printf("Bloom created. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGaugeQuicken, time);
					
					break;
				
				default:
					break;
			}
			
			break;
			
			
		// Quicken Aura (Underlying Electro)	
		case 10:
			if(auraGaugeQuicken > 0 && auraGaugeElectro <= 0){
				auraType = 9;
				reaction(applyElement, applyGauge);
				break;
			}
			
			switch (applyElement){
				// QAUE + Electro = Aggravate
				case 3:
					if(applyGauge > auraGaugeElectro) auraGaugeElectro = 0.8 * applyGauge;
					aggravates++;
					printf("Aggravate triggered. %s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
					if(auraGaugeElectro > 0) printf("and Underlying Electro Aura of %.3fU ", auraGaugeElectro);
					printf("remain at %.2fs\n", time);
					
					break;
					
				// QAUE + Dendro = Spread
				case 4:
					// triggers quicken and spread
					printf("Spread triggered. %s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
					
					spreads++;
					newAuraGaugeQuicken = auraGaugeElectro < applyGauge ? auraGaugeElectro : applyGauge;
					if(newAuraGaugeQuicken > auraGaugeQuicken){
						auraGaugeQuicken = newAuraGaugeQuicken;
						decayRateQuicken = 1 / (auraGaugeQuicken * 5 + 6);
					}
					
					if(applyGauge < auraGaugeElectro){
						auraGaugeElectro = auraGauge - applyGauge;
						decayRate = 1/(2.5 * auraGaugeElectro + 7);
					} else auraGaugeElectro = 0.0;
					
					if(auraGaugeElectro > 0) printf("and Underlying Electro Aura of %.3fU ", auraGaugeElectro);
					printf("remain at %.2fs\n", time);
					
					break;
					
				// QAUE + Hydro = Reverse Bloom (+ Electro-Charged if possible)
				case 5:
					auraGaugeHydro = applyGauge - 2.0 * auraGaugeQuicken;
					auraGaugeQuicken -= 0.5 * applyGauge;
					blooms++;
//					printf("%.3f %.3f %.3f\n", auraGaugeQuicken, auraGaugeHydro, applyGauge);
					if(auraGaugeQuicken <= 0){
						printf("Bloom created. Quicken expired. ");
						
						if(auraGaugeElectro <= 0){
							printf("Electro Aura expired. ");
							auraType = 8;
							auraGauge = 0.0;
						}
						else {
							if(auraGaugeHydro > 0 && auraGaugeElectro > 0){
								puts("");
								electroChargedTick();
//								break;
							}
							printf("Electro Aura of %.3fU and Hydro Aura of %.3fU remain at %.2fs\n", auraGaugeElectro, auraGaugeHydro, time);
							auraType = 12;
							auraGauge = auraGaugeElectro;
						}
						
						break;
					}
					
					printf("Bloom created. %s Aura of %.3fU and Electro Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGaugeQuicken, auraGaugeElectro, time);
					
					break;
				
				default:
					break;
			}
			
			break;
			
			
		
		// Quicken Aura (Underlying Dendro)	
		case 11:
			if(auraGaugeQuicken > 0 && auraGaugeDendro <= 0){
				auraType = 9;
				reaction(applyElement, applyGauge);
				break;
			}
			
			switch (applyElement){
				// QAUD + Electro = Aggravate
				case 3:
					// triggers quicken and aggravate
					aggravates++;
					printf("Aggravate triggered. %s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
					
					newAuraGaugeQuicken = auraGaugeDendro < applyGauge ? auraGaugeDendro : applyGauge;
					if(newAuraGaugeQuicken > auraGaugeQuicken){
						auraGaugeQuicken = newAuraGaugeQuicken;
						decayRateQuicken = 1 / (auraGaugeQuicken * 5 + 6);
					}
					
					if(applyGauge < auraGaugeDendro){
						auraGaugeDendro = auraGauge - applyGauge;
						decayRate = 1/(2.5 * auraGaugeDendro + 7);
					} else auraGaugeDendro = 0.0;
					
					if(auraGaugeElectro > 0) printf("and Underlying Dendro Aura of %.3fU ", auraGaugeDendro);
					printf("remain at %.2fs\n", time);
					
					break;
					
				// QAUD + Dendro = Spread
				case 4:
					spreads++;
					printf("Spread triggered. %s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
					
					if(applyGauge > auraGaugeDendro) auraGaugeDendro = 0.8 * applyGauge;
					
					if(auraGaugeDendro > 0) printf("and Underlying Dendro Aura of %.3fU ", auraGaugeDendro);
					printf("remain at %.2fs\n", time);
					
					break;
					
				// QAUD + Hydro = Reverse Bloom
				case 5:
					auraGaugeHydro = applyGauge - 2.0 * auraGaugeQuicken;
					auraGaugeQuicken -= 0.5 * applyGauge;
					auraGaugeDendro -= 0.5 * applyGauge;
					blooms++;
//					printf("%.3f %.3f %.3f\n", auraGaugeQuicken, auraGaugeHydro, applyGauge);
					if(auraGaugeQuicken <= 0){
						printf("Bloom created. Quicken expired. ");
						if(auraGaugeDendro <= 0){
							printf("Dendro Aura expired. ");
							auraType = 8;
							auraGauge = 0.0;
						}
						else {
							printf("Dendro Aura of %.3fU remain at %.2fs\n", auraGaugeDendro, time);
							auraType = 4;
							auraGauge = auraGaugeDendro;
						}
						
						break;
					}
					
					
					
					printf("Bloom created. %s Aura of %.3fU and Dendro Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGaugeQuicken, auraGaugeDendro, time);
					
					break;
				
				default:
					break;
			}
			
			break;
			
			
		
			
			
			
			
		// Electro-Charged Aura
		case 12:
			switch (applyElement){
				// Electro-Charged Aura + Electro = Electro-Charged Aura
				case 3:
					applyGauge *= 0.8;
					auraGaugeElectro = auraGaugeElectro > applyGauge ? auraGaugeElectro : applyGauge;
					
					if(auraGaugeHydro <= 0){
						auraType = 3;
						auraGauge = auraGaugeElectro;
						printf("%s Aura of %.3fU Electro remain at %.2fs\n", elements[auraType], auraGaugeElectro, time);
					} else {
						printf("%s Aura (%.3fU Hydro, %.3fU Electro) refreshed at %.2fs\n", elements[auraType], auraGaugeHydro, auraGaugeElectro, time);
					}
					
					break;
					
				// Electro-Charged Aura + Dendro = Quicken + Forward Bloom
				case 4:
					remainingGauge = applyGauge - auraGaugeElectro;
					auraGaugeQuicken = auraGaugeElectro < applyGauge ? auraGaugeElectro : applyGauge;
					auraGauge = 0.0;
					auraGaugeElectro = 0.0;
					decayRate = 1 / (auraGaugeQuicken * 5 + 6);
					
					if(remainingGauge > 0) auraType = 11;
					else auraType = 9;
					
					printf("Quicken activated. Quicken Aura of %.3fU applied at %.2fs\n", auraGaugeQuicken, time);
					
					if(remainingGauge > 0){
						auraGaugeHydro -= 2.0 * remainingGauge;
						
						blooms++;
						printf("Bloom created. %s Aura of %.3fU ", elements[auraType], auraGaugeQuicken);
						if(auraGaugeHydro > 0) printf("and Hydro Aura of %.3fU ", auraGaugeHydro);
						printf("remain at %.2fs\n", time);
						
						if(auraGaugeHydro > 0){
							auraType = 5;
							auraGauge = auraGaugeHydro;
							reaction(4, auraGaugeHydro);
						} else {
							auraType = 11;
							auraGaugeDendro = remainingGauge;
							decayRate = 1/(2.5 * auraGaugeDendro + 7);
						}
					}
					
					break;
					
				// Electro-Charged Aura + Hydro = Electro-Charged Aura
				case 5:
					applyGauge *= 0.8;
					auraGaugeHydro = auraGaugeHydro > applyGauge ? auraGaugeHydro : applyGauge;
					
					if(auraGaugeElectro <= 0){
						auraType = 5;
						auraGauge = auraGaugeHydro;
						printf("%s Aura of %.3fU Hydro remain at %.2fs\n", elements[auraType], auraGaugeHydro, time);
					} else {
						printf("%s Aura (%.3fU Hydro, %.3fU Electro) refreshed at %.2fs\n", elements[auraType], auraGaugeHydro, auraGaugeElectro, time);
					}
					
					
					break;
				
				default:
					break;
			}
			
			break;
			
			
			
			
			
			
		default:
			break;
	}
	
	puts("");
}


void addReaction(double t, double ae, double ag){
	prevTime = time;
	time = t;
	reaction(ae, ag);
}

int main(){
	insertData();
	
	puts("~~~~~~~~~~~~~~~~~~");
	puts("| FIRST ROTATION |");
	puts("~~~~~~~~~~~~~~~~~~");
	
	//0.87 raiden skill
	puts("Raiden Skill");
	addReaction(0.87, 3, 1.0);
	raidenSkill();
	dendroRes = 1.1;
	
	//1.17 raiden skill tick
	puts("Raiden Skill");
	time = 1.17;
	raidenSkill();
	
	//1.43 furina burst (cast)
	
	//2.42 furina seahorse
	activeChar = 4;
	puts("Furina Seahorse");
	addReaction(2.42, 5, 1.0);
	furinaSeahorse();
	
	//2.91 furina octopus
	puts("Furina Octopus");
	addReaction(2.91, 5, 1.0);
	furinaOctopus();
	
	//2.91 furina crab
	puts("Furina Crab");
	addReaction(2.91, 5, 1.0);
	furinaCrab();
	
	//2.95 furina burst
	puts("Furina Burst");
	addReaction(2.95, 5, 1.0);
	furina.damage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus - 70.0/100) * enemyRes * enemyDef;
	totalDamage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus - 70.0/100) * enemyRes * enemyDef;
	printCurrHP();
	furinaBurstActive = true;
	prevFanfare = 0;
	fanfare = 0;
	
	//3.03 raiden skill tick
	puts("Raiden Skill");
	time = 3.03;
	raidenSkill();
	
	//3.72 baizhu skill
	activeChar = 3;
	puts("Baizhu Skill");
	addReaction(3.72, 4, 1.0);
	baizhu.damage += 3 * (134.6/100 * baizhu.ATK) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	totalDamage += 3 * (134.6/100 * baizhu.ATK) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	printCurrHP();
	
	alhaitham.EM += 30;
	raiden.EM += 30;
	baizhu.EM += 30;
	furina.EM += 30;
	
	//4.05 furina seahorse
	puts("Furina Seahorse");
	time = 4.05;
	furinaSeahorse();
	
	//4.05 baizhu burst (cast)
	
	//4.60 raiden skill tick
	puts("Raiden Skill");
	addReaction(4.60, 3, 1.0);
	raidenSkill();
	
	alhaitham.EM += 20;
	raiden.EM += 20;
	baizhu.EM += 20;
	furina.EM += 20;
	
	//5.68 raiden skill tick
	puts("Raiden Skill");
	time = 5.68;
	raidenSkill();
	
	//5.76 furina seahorse
	puts("Furina Seahorse");
	addReaction(5.76, 5, 1.0);
	furinaSeahorse();
	
	//5.91 baizhu burst (finish)
	
	//6.02 alhaitham burst (cast)
	activeChar = 1;
	
	//6.14 furina octopus
	puts("Furina Octopus");
	time = 6.14;
	furinaOctopus();
	
	//6.47 baizhu skill heal
	puts("Baizhu Skill Heal");
	time = 6.47;
	baizhuSkillHeal();
	
	//7.36 furina seahorse
	puts("Furina Seahorse");
	time = 7.36;
	furinaSeahorse();
	
	//7.41 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 7.41;
	baizhuBurstHeal();
	
	//7.58 alhaitham burst tick
	puts("Alhaitham Burst Tick");
	addReaction(7.58, 4, 1.0);
	alhaithamBurst(0);
	
	//7.67 raiden skill tick
	puts("Raiden Skill");
	time = 7.67;
	raidenSkill();
	
	//7.91 baizhu pamber heal
	puts("Baizhu Prototype Amber Heal");
	time = 7.91;
	baizhuProtoHeal();
	
	//7.93 alhaitham burst tick
	puts("Alhaitham Burst Tick");
	time = 7.93;
	alhaithamBurst(0);
	
	//8.04 furina crab
	puts("Furina Crab");
	addReaction(8.04, 5, 1.0);
	furinaCrab();
	
	//8.25 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(8.25, 4, 1.0);
	baizhuBurstDamage();
	
	//8.28 alhaitham burst tick
	puts("Alhaitham Burst Tick");
	time = 8.28;
	alhaithamBurst(0);
	
	//8.38 baizhu ohc pop
	baizhuOHC();
	
	//8.62 raiden skill tick
	puts("Raiden Skill");
	time = 8.62;
	raidenSkill();
	
	//8.63 alhaitham burst tick
	puts("Alhaitham Burst Tick");
	addReaction(8.63, 4, 1.0);
	alhaithamBurst(0);
	
	//8.88 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//9.07 furina seahorse
	puts("Furina Seahorse");
	addReaction(9.07, 5, 1.0);
	furinaSeahorse();
	
	//9.43 alhaitham n4
	puts("Alhaitham N4");
	time = 9.43;
	alhaithamN4(0);
	
	//9.45 furina octopus
	puts("Furina Octopus");
	addReaction(9.45, 5, 1.0);
	furinaOctopus();
	
	//9.52 raiden skill tick
	puts("Raiden Skill");
	addReaction(9.52, 3, 1.0);
	raidenSkill();
	
	//9.91 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 9.91;
	baizhuBurstHeal();
	
	//9.91 baizhu pamber heal
	puts("Baizhu Prototype Amber Heal");
	time = 9.91;
	baizhuProtoHeal();
	
	//9.97 alhaitham mirror
	puts("Alhaitham Projection");
	alhaithamLofiActive = 1;
	alhaithamLofiStacks = 28;
	addReaction(9.97, 4, 1.0);
	alhaithamMirror(0);
	alhaitham.EM += 100;
	alhaitham.ATK += 855 * 14.0/100;
	
	//10.12 alhaitham mirror
	puts("Alhaitham Projection");
	time = 10.12;
	alhaithamMirror(0);
	
	//10.17 alhaitham n5
	puts("Alhaitham N5");
	time = 10.17;
	alhaithamN5(0);
	
	//10.28 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(10.28, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//10.66 furina seahorse
	puts("Furina Seahorse");
	time = 10.66;
	furinaSeahorse();
	
	//10.68 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(10.68, 4, 1.0);
	baizhuBurstDamage();
	
	//10.77 raiden skill tick
	puts("Raiden Skill");
	time = 10.77;
	raidenSkill();
	
	//10.95 alhaitham n1
	puts("Alhaitham N1");
	addReaction(10.95, 4, 1.0);
	alhaithamN1(alhaithamSpread);
	
	//11.30 alhaitham n2
	puts("Alhaitham N2");
	time = 11.30;
	alhaithamN2(0);
	
	//11.85 raiden skill tick
	puts("Raiden Skill");
	time = 11.85;
	raidenSkill();
	
	//11.77 alhaitham n3a
	puts("Alhaitham N3a");
	time = 11.77;
	alhaithamN3(0);
	
	//11.83 alhaitham mirror
	puts("Alhaitham Projection");
	time = 11.83;
	alhaithamMirror(0);
	
	//11.91 baizhu pamber heal
	puts("Baizhu Prototype Amber Heal");
	time = 11.91;
	baizhuProtoHeal();
	
	//11.98 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(11.98, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//12.00 alhaitham n3b
	puts("Alhaitham N3b");
	addReaction(12.00, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN3(alhaithamSpread);
	
	//12.15 alhaitham mirror
	puts("Alhaitham Projection");
	time = 12.15;
	alhaithamMirror(0);
	
	//12.19 furina seahorse
	puts("Furina Seahorse");
	addReaction(12.19, 5, 1.0);
	furinaSeahorse();
	
	//12.41 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 12.41;
	baizhuBurstHeal();
	
	//12.65 alhaitham charge
	puts("Alhaitham Charge A");
	addReaction(12.65, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamCharge(alhaithamSpread);
	
	//12.78 alhaitham charge
	puts("Alhaitham Charge B");
	time = 12.78;
	alhaithamCharge(0);
	
	//12.71 furina octopus
	puts("Furina Octopus");
	time = 12.71;
	furinaOctopus();
	
	//12.87 raiden skill tick
	puts("Raiden Skill");
	addReaction(12.87, 3, 1.0);
	raidenSkill();
	
	//13.07 baizhu ohc pop
	baizhuOHC();
	
	//13.12 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(13.12, 4, 1.0);
	baizhuBurstDamage();
	
	//13.22 furina crab
	puts("Furina Crab");
	addReaction(13.22, 5, 1.0);
	furinaCrab();
	
	//13.35 alhaitham n1
	puts("Alhaitham N1");
	time = 13.35;
	alhaithamN1(0);
	
	//13.57 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//13.70 alhaitham n2
	puts("Alhaitham N2");
	addReaction(13.70, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN2(alhaithamSpread);
	
	//13.78 raiden skill tick
	puts("Raiden Skill");
	time = 13.78;
	raidenSkill();
	
	//13.83 furina seahorse
	puts("Furina Seahorse");
	time = 13.83;
	furinaSeahorse();
	
	//13.88 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(13.88, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//14.03 alhaitham mirror
	puts("Alhaitham Projection");
	time = 14.03;
	alhaithamMirror(0);
	
	//14.17 alhaitham n3a
	puts("Alhaitham N3a");
	time = 14.17;
	alhaithamN3(0);
	
	//14.20 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(14.20, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//14.40 alhaitham n3b
	puts("Alhaitham N3b");
	time = 14.40;
	alhaithamN3(0);
	
	//14.91 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 14.91;
	baizhuBurstHeal();
	
	//15.07 alhaitham n4
	puts("Alhaitham N4");
	addReaction(15.07, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN4(alhaithamSpread);
	
	//15.15 raiden skill tick
	puts("Raiden Skill");
	time = 15.15;
	raidenSkill();
	
	//15.43 furina seahorse
	puts("Furina Seahorse");
	addReaction(15.43, 5, 1.0);
	furinaSeahorse();
	
	//15.55 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(15.55, 4, 1.0);
	baizhuBurstDamage();
	
	//15.80 alhaitham n5
	puts("Alhaitham N5");
	time = 15.80;
	alhaithamN5(0);
	
	//15.91 furina octopus
	puts("Furina Octopus");
	addReaction(15.91, 5, 1.0);
	furinaOctopus();
	
	//16.33 alhaitham mirror
	puts("Alhaitham Projection");
	time = 16.33;
	alhaithamMirror(0);
	
	//16.42 raiden skill tick
	puts("Raiden Skill");
	addReaction(16.42, 3, 1.0);
	raidenSkill();
	
	//16.48 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(16.48, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//16.58 alhaitham n1
	puts("Alhaitham N1");
	time = 16.58;
	alhaithamN1(0);
	
	//16.65 alhaitham mirror
	puts("Alhaitham Projection");
	time = 16.65;
	alhaithamMirror(0);
	
	//16.93 alhaitham n2
	puts("Alhaitham N2");
	time = 16.93;
	alhaithamN2(0);
	
	//17.02 furina seahorse
	puts("Furina Seahorse");
	time = 17.02;
	furinaSeahorse();
	
	//17.40 alhaitham n3a
	puts("Alhaitham N3a");
	time = 17.40;
	alhaithamN3(0);
	
	//17.41 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 17.41;
	baizhuBurstHeal();
	
	//17.48 raiden skill tick
	puts("Raiden Skill");
	time = 17.48;
	raidenSkill();
	
	//17.63 alhaitham n3b
	puts("Alhaitham N3b");
	time = 17.63;
	addReaction(17.63, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN3(alhaithamSpread);
	
	//17.98 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(17.98, 4, 1.0);
	baizhuBurstDamage();
	
	//18.07 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(18.07, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//18.08 baizhu ohc pop
	baizhuOHC();
	
	//18.22 alhaitham mirror
	puts("Alhaitham Projection");
	time = 18.22;
	alhaithamMirror(0);
	
	//18.30 alhaitham n4
	puts("Alhaitham N4");
	time = 18.30;
	alhaithamN4(0);
	
	//18.38 raiden skill tick
	puts("Raiden Skill");
	time = 18.38;
	raidenSkill();
	
	//18.46 furina crab
	puts("Furina Crab");
	addReaction(18.46, 5, 1.0);
	furinaCrab();
	
	//18.58 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//18.66 furina seahorse
	puts("Furina Seahorse");
	addReaction(18.66, 5, 1.0);
	furinaSeahorse();
	
	//19.03 alhaitham n5
	puts("Alhaitham N5");
	time = 19.03;
	alhaithamN5(0);
	
	//19.07 furina octopus
	puts("Furina Octopus");
	time = 19.07;
	furinaOctopus();
	
	//19.82 alhaitham n1
	puts("Alhaitham N1");
	addReaction(19.82, 4, 1.0);
	alhaithamN1(0);
	
	//19.90 raiden skill tick
	puts("Raiden Skill");
	addReaction(19.90, 3, 1.0);
	raidenSkill();
	
	//19.91 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 19.91;
	baizhuBurstHeal();
	
	//20.17 alhaitham n2
	puts("Alhaitham N2");
	time = 20.17;
	alhaithamN2(0);
	
	//20.27 furina seahorse
	puts("Furina Seahorse");
	time = 20.27;
	furinaSeahorse();
	
	//20.52 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(20.52, 4, 1.0);
	baizhuBurstDamage();
	
	//20.63 alhaitham n3a
	puts("Alhaitham N3a");
	time = 20.63;
	alhaithamN3(0);
	
	//20.87 alhaitham n3b
	puts("Alhaitham N3b");
	addReaction(20.87, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN3(alhaithamSpread);
	
	//20.95 raiden skill tick
	puts("Raiden Skill");
	time = 20.95;
	raidenSkill();
	
	//20.95 furina burst expires
	furinaBurstActive = false;
	
	alhaitham.DMGBonus -= (fanfare) * 0.23/100;
	raiden.DMGBonus -= (fanfare) * 0.23/100;
	baizhu.DMGBonus -= (fanfare) * 0.23/100;
	furina.DMGBonus -= (fanfare) * 0.23/100;
	
	alhaitham.incomingHB -= (fanfare) * 0.09/100;
	raiden.incomingHB -= (fanfare) * 0.09/100;
	baizhu.incomingHB -= (fanfare) * 0.09/100;
	furina.incomingHB -= (fanfare) * 0.09/100;
	
	//21.53 alhaitham n4
	puts("Alhaitham N4");
	time = 21.53;
	alhaithamN4(0);
	
	//21.79 furina seahorse
	puts("Furina Seahorse");
	addReaction(21.79, 5, 1.0);
	furinaSeahorse();
	
	//22.27 alhaitham n5
	puts("Alhaitham N5");
	time = 22.27;
	alhaithamN5(0);
	
	//22.30 furina octopus
	puts("Furina Octopus");
	addReaction(22.30, 5, 1.0);
	furinaOctopus();
	
	//22.35 raiden skill tick
	puts("Raiden Skill");
	time = 22.35;
	raidenSkill();
	
	//23.05 baizhu ohc pop
	baizhuOHC();
	
	puts("");
	
	viewData();
	
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", alhaitham.name, alhaitham.currHP * 100, alhaitham.currHP * alhaitham.HP, alhaitham.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", raiden.name, raiden.currHP * 100, raiden.currHP * raiden.HP, raiden.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", baizhu.name, baizhu.currHP * 100, baizhu.currHP * baizhu.HP, baizhu.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", furina.name, furina.currHP * 100, furina.currHP * furina.HP, furina.HP);
	
	puts("");
	
	printf("Fanfare: %.1f\n", fanfare);
	printf("Total Damage: %.1f\n", totalDamage);
	printf("Hyperblooms: %d\n", hyperblooms);
	printf("Alhaitham Damage: %.1f\n", alhaitham.damage);
	printf("Raiden Damage: %.1f\n", raiden.damage);
	printf("Baizhu Damage: %.1f\n", baizhu.damage);
	printf("Furina Damage: %.1f\n", furina.damage);
	
	puts("");
	printf("Damage per Second: %.1f\n", totalDamage / 23.0);
	printf("Alhaitham DPS: %.1f\n", alhaitham.damage / 23.0);
	printf("Raiden DPS: %.1f\n", raiden.damage / 23.0);
	printf("Baizhu DPS: %.1f\n", baizhu.damage / 23.0);
	printf("Furina DPS: %.1f\n", furina.damage / 23.0);
	
	puts("");
	printf("Alhaitham Contribution: %.1f%%\n", alhaitham.damage / totalDamage * 100);
	printf("Raiden Contribution: %.1f%%\n", raiden.damage / totalDamage * 100);
	printf("Baizhu Contribution: %.1f%%\n", baizhu.damage / totalDamage * 100);
	printf("Furina Contribution: %.1f%%\n", furina.damage / totalDamage * 100);
	
	fanfare = 0;
	prevFanfare = 0;
	hyperblooms = 0;
	
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	
	
	
	//2nd rot
	
	puts("~~~~~~~~~~~~~~~~~~~");
	puts("| SECOND ROTATION |");
	puts("~~~~~~~~~~~~~~~~~~~");
	
	totalDamage = 0.0;
	alhaitham.damage = 0.0;
	raiden.damage = 0.0;
	baizhu.damage = 0.0;
	furina.damage = 0.0;
	
	
	
	//23.40 raiden skill
	activeChar = 2;
	puts("Raiden Skill");
	addReaction(23.40, 3, 1.0);
	raidenSkill();
	
	//23.46 furina seahorse
	puts("Furina Seahorse");
	time = 23.46;
	furinaSeahorse();
	
	//23.55 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//23.65 furina crab
	puts("Furina Crab");
	addReaction(23.65, 5, 1.0);
	furinaCrab();
	
	//24.10 raiden skill tick
	puts("Raiden Skill");
	time = 24.10;
	raidenSkill();
	
	///24.30 furina burst (cast)
	activeChar = 4;
	
	//25.35 furina seahorse
	puts("Furina Seahorse");
	addReaction(25.35, 5, 1.0);
	furinaSeahorse();
	
	//25.84 furina octopus
	puts("Furina Octopus");
	time = 25.84;
	furinaOctopus();
	
	//25.84 furina crab
	puts("Furina Crab");
	addReaction(25.84, 5, 1.0);
	furinaCrab();
	
	//25.88 furina burst
	puts("Furina Burst");
	addReaction(25.88, 5, 1.0);
	furina.damage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus - 70.0/100) * enemyRes * enemyDef;
	totalDamage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus - 70.0/100) * enemyRes * enemyDef;
	printCurrHP();
	furinaBurstActive = true;
	prevFanfare = 0;
	fanfare = 0;
	
	//25.97 raiden skill tick
	puts("Raiden Skill");
	time = 25.97;
	raidenSkill();
	
	//26.63 baizhu skill
	activeChar = 3;
	puts("Baizhu Skill");
	addReaction(26.63, 4, 1.0);
	baizhu.damage += 3 * (134.6/100 * baizhu.ATK) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	totalDamage += 3 * (134.6/100 * baizhu.ATK) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	printCurrHP();
	
	//26.97 baizhu burst (cast)
	
	//26.98 furina seahorse
	puts("Furina Seahorse");
	time = 26.98;
	furinaSeahorse();
	
	//27.05 raiden skill tick
	puts("Raiden Skill");
	addReaction(27.05, 3, 1.0);
	raidenSkill();
	
	//27.98 raiden skill tick
	puts("Raiden Skill");
	time = 27.98;
	raidenSkill();
	
	//28.69 furina seahorse
	puts("Furina Seahorse");
	addReaction(28.69, 5, 1.0);
	furinaSeahorse();
	
	//28.83 baizhu burst (finish)
	
	//29.07 furina octopus
	puts("Furina Octopus");
	addReaction(29.07, 5, 1.0);
	furinaOctopus();
	
	//29.38 baizhu skill heal
	puts("Baizhu Skill Heal");
	time = 29.38;
	baizhuSkillHeal();
	
	//29.40 alhaitham skill
	activeChar = 1;
	puts("Alhaitham Skill");
	addReaction(29.40, 4, 1.0);
	alhaithamSkill(0);
	
	//29.48 raiden skill tick
	puts("Raiden Skill");
	time = 29.48;
	raidenSkill();
	
	//30.29 furina seahorse
	puts("Furina Seahorse");
	time = 30.29;
	furinaSeahorse();
	
	//30.32 alhaitham low plunge
	puts("Alhaitham Low Plunge");
	addReaction(30.32, 4, 1.0);
	alhaithamPlunge(0);
	
	//30.33 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 30.33;
	baizhuBurstHeal();
	
	//30.40 raiden skill tick
	puts("Raiden Skill");
	addReaction(30.40, 3, 1.0);
	raidenSkill();
	
	//30.68 alhaitham n1
	puts("Alhaitham N1");
	addReaction(30.68, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN1(alhaithamSpread);
	
	//30.83 baizhu pamber heal
	puts("Baizhu Prototype Amber Heal");
	time = 30.83;
	baizhuProtoHeal();
	
	//30.85 alhaitham mirror
	puts("Alhaitham Projection");
	alhaithamLofiActive = 1;
	alhaithamLofiStacks = 28;
	time = 30.85;
	alhaithamMirror(0);
	
	//30.97 furina crab
	puts("Furina Crab");
	addReaction(30.97, 5, 1.0);
	furinaCrab();
	
	//31.00 alhaitham mirror
	puts("Alhaitham Projection");
	alhaithamLofiActive = 1;
	alhaithamLofiStacks = 28;
	addReaction(31.00, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//31.03 alhaitham n2
	puts("Alhaitham N2");
	time = 31.03;
	alhaithamN2(0);
	
	//31.17 alhaitham mirror
	puts("Alhaitham Projection");
	time = 31.17;
	alhaithamMirror(0);
	
	//31.17 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(31.17, 4, 1.0);
	baizhuBurstDamage();
	
	//31.30 baizhu ohc pop
	baizhuOHC();
	
	//31.38 raiden skill tick
	puts("Raiden Skill");
	time = 31.38;
	raidenSkill();
	
	//31.50 alhaitham n3a
	puts("Alhaitham N3a");
	time = 31.50;
	alhaithamN3(0);
	
	//31.73 alhaitham n3b
	puts("Alhaitham N3b");
	addReaction(31.73, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN3(alhaithamSpread);
	
	//31.80 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//32.00 furina seahorse
	puts("Furina Seahorse");
	addReaction(32.00, 5, 1.0);
	furinaSeahorse();
	
	//32.40 alhaitham n4
	puts("Alhaitham N4");
	time = 32.40;
	alhaithamN4(0);
	
	//32.38 furina octopus
	puts("Furina Octopus");
	time = 32.38;
	furinaOctopus();
	
	//32.48 raiden skill tick
	puts("Raiden Skill");
	time = 32.48;
	raidenSkill();
	
	//32.83 baizhu pamber heal
	puts("Baizhu Prototype Amber Heal");
	time = 32.83;
	baizhuProtoHeal();
	
	//32.83 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 32.83;
	baizhuBurstHeal();
	
	//32.93 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(32.93, 4, 1.0);
	alhaithamMirror(0);
	
	//33.08 alhaitham mirror
	puts("Alhaitham Projection");
	time = 33.08;
	alhaithamMirror(0);
	
	//33.13 alhaitham n5
	puts("Alhaitham N5");
	time = 33.13;
	alhaithamN5(0);
	
	//33.25 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(33.25, 4, 1.0);
	alhaithamMirror(0);
	
	//33.59 furina seahorse
	puts("Furina Seahorse");
	time = 33.59;
	furinaSeahorse();
	
	//33.60 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(33.60, 4, 1.0);
	baizhuBurstDamage();
	
	//33.68 raiden skill tick
	puts("Raiden Skill");
	addReaction(33.68, 3, 1.0);
	raidenSkill();
	
	//33.92 alhaitham n1
	puts("Alhaitham N1");
	addReaction(33.92, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN1(alhaithamSpread);
	
	//34.27 alhaitham n2
	puts("Alhaitham N2");
	time = 34.27;
	alhaithamN2(0);
	
	//34.70 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(34.70, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//34.73 alhaitham n3a
	puts("Alhaitham N3a");
	time = 34.73;
	alhaithamN3(0);
	
	//34.78 raiden skill tick
	puts("Raiden Skill");
	time = 34.78;
	raidenSkill();
	
	//34.83 baizhu pamber heal
	puts("Baizhu Prototype Amber Heal");
	time = 34.83;
	baizhuProtoHeal();
	
	//34.85 alhaitham mirror
	puts("Alhaitham Projection");
	time = 34.85;
	alhaithamMirror(0);
	
	//34.97 alhaitham n3b
	puts("Alhaitham N3b");
	addReaction(34.97, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN3(alhaithamSpread);
	
	//35.12 furina seahorse
	puts("Furina Seahorse");
	addReaction(35.12, 5, 1.0);
	furinaSeahorse();
	
	//35.33 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 35.33;
	baizhuBurstHeal();
	
	//35.63 alhaitham n4
	puts("Alhaitham N4");
	time = 35.63;
	alhaithamN4(0);
	
	//35.64 furina octopus
	puts("Furina Octopus");
	addReaction(35.64, 5, 1.0);
	furinaOctopus();
	
	//35.72 raiden skill tick
	puts("Raiden Skill");
	time = 35.72;
	raidenSkill();
	
	//35.98 baizhu ohc pop
	baizhuOHC();
	
	//36.03 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(36.03, 4, 1.0);
	baizhuBurstDamage();
	
	//36.15 furina crab
	puts("Furina Crab");
	addReaction(36.15, 5, 1.0);
	furinaCrab();
	
	//36.37 alhaitham n5
	puts("Alhaitham N5");
	time = 36.37;
	alhaithamN5(0);
	
	//36.48 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//36.76 furina seahorse
	puts("Furina Seahorse");
	time = 36.76;
	furinaSeahorse();
	
	//36.83 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(36.83, 4, 1.0);
	alhaithamMirror(0);
	
	//36.92 raiden skill tick
	puts("Raiden Skill");
	addReaction(36.92, 3, 1.0);
	raidenSkill();
	
	//36.98 alhaitham mirror
	puts("Alhaitham Projection");
	time = 36.98;
	alhaithamMirror(0);
	
	//37.15 alhaitham n1
	puts("Alhaitham N1");
	addReaction(37.15, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN1(alhaithamSpread);
	
	//37.50 alhaitham n2
	puts("Alhaitham N2");
	time = 37.50;
	alhaithamN2(0);
	
	//37.83 baizhu burst heal
	puts("Baizhu Burst Heal");
	time = 37.83;
	baizhuBurstHeal();
	
	//37.97 alhaitham n3a
	puts("Alhaitham N3a");
	time = 37.97;
	alhaithamN3(0);
	
	//38.05 raiden skill tick
	puts("Raiden Skill");
	time = 38.05;
	raidenSkill();
	
	//38.20 alhaitham n3b
	puts("Alhaitham N3b");
	addReaction(38.20, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamN3(alhaithamSpread);
	
	//38.36 furina seahorse
	puts("Furina Seahorse");
	addReaction(38.36, 5, 1.0);
	furinaSeahorse();
	
	//38.47 baizhu burst tick
	puts("Baizhu Burst Tick");
	addReaction(38.47, 4, 1.0);
	baizhuBurstDamage();
	
	//38.84 furina octopus
	puts("Furina Octopus");
	time = 38.84;
	furinaOctopus();
	
	//38.87 alhaitham n4
	puts("Alhaitham N4");
	time = 38.87;
	alhaithamN4(0);
	
	//38.87 alhaitham mirror
	puts("Alhaitham Projection");
	addReaction(38.87, 4, 1.0);
	alhaithamSpread = 1.25 * 1446.85 * (1 + (5 * alhaitham.EM)/(1200 + alhaitham.EM));
	alhaithamMirror(alhaithamSpread);
	
	//38.95 raiden skill tick
	puts("Raiden Skill");
	time = 38.95;
	raidenSkill();
	
	//39.60 alhaitham n5
	puts("Alhaitham N5");
	time = 39.60;
	alhaithamN5(0);
	
	//39.95 furina seahorse
	puts("Furina Seahorse");
	time = 39.95;
	furinaSeahorse();
	
	
	
	
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", alhaitham.name, alhaitham.currHP * 100, alhaitham.currHP * alhaitham.HP, alhaitham.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", raiden.name, raiden.currHP * 100, raiden.currHP * raiden.HP, raiden.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", baizhu.name, baizhu.currHP * 100, baizhu.currHP * baizhu.HP, baizhu.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", furina.name, furina.currHP * 100, furina.currHP * furina.HP, furina.HP);
	
	puts("");
	
	printf("Fanfare: %.1f\n", fanfare);
	printf("Total Damage: %.1f\n", totalDamage);
	printf("Hyperblooms: %d\n", hyperblooms);
	printf("Alhaitham Damage: %.1f\n", alhaitham.damage);
	printf("Raiden Damage: %.1f\n", raiden.damage);
	printf("Baizhu Damage: %.1f\n", baizhu.damage);
	printf("Furina Damage: %.1f\n", furina.damage);
	
	puts("");
	printf("Damage per Second: %.1f\n", totalDamage / 17.0);
	printf("Alhaitham DPS: %.1f\n", alhaitham.damage / 17.0);
	printf("Raiden DPS: %.1f\n", raiden.damage / 17.0);
	printf("Baizhu DPS: %.1f\n", baizhu.damage / 17.0);
	printf("Furina DPS: %.1f\n", furina.damage / 17.0);
	
	puts("");
	printf("Alhaitham Contribution: %.1f%%\n", alhaitham.damage / totalDamage * 100);
	printf("Raiden Contribution: %.1f%%\n", raiden.damage / totalDamage * 100);
	printf("Baizhu Contribution: %.1f%%\n", baizhu.damage / totalDamage * 100);
	printf("Furina Contribution: %.1f%%\n", furina.damage / totalDamage * 100);
	
	
    return 0;
}
