#include <stdio.h>
#include <string.h>

double dendroRes = 0.9; // dendro
double enemyRes = 0.9; // electro and hydro
double enemyDef = 0.5;

double totalDamage = 0.0;
double prevFanfare = 0.0;
double fanfare = 0.0;

double cynoAgg = 0.0;
double nahidaSpread = 0.0;

int activeChar = 1;

double baizhuOHCHeal = 0.0;
double baizhuBloomBuff = 0.0;
double baizhuAdditiveBuff = 0.0;
double nahidaEMBuff = 0.0;
double nahidaWanderingEvenstarBuff = 0.0;
double alliesWanderingEvenstarBuff = 0.0;
bool furinaBurstActive = false;
//int cynoStaffStacks = 0;
//double cynoStaffBuff = 0;

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
} cyno, nahida, baizhu, furina;

void insertData(){
	strcpy(cyno.name, "Cyno"); // 1.2k atk, 397 em | fjords, 4p tf, EM/Elec/Crit, 1/9/9
	cyno.HP = 12491 * (1 + 0.0992) + 4780 + 507.88; // base + substat
	cyno.ATK = (318 + 510) * (1 + 0.0992) + 311 + 33.08; // base + sig + substat + substat + substat
	cyno.ER = 1 + 0.4408;
	cyno.EM = 187 + 39.64 + 50 + 120; // em sands + substat + dendro resonance + fjords passive
	cyno.DMGBonus = 46.6/100.0 + 15/100.0; // electro gob + 2p tf
	cyno.critRate = 5/100.0 + 27.6/100.0 + 0.3972; // base + fjords + substat
	cyno.critDMG = 50/100.0 + 38.4/100 + 62.2/100.0 + 0.3972; // base + ascension + cd circlet + substat
	cyno.currHP = 100.0/100.0;
	cyno.HB = 0.0/100.0;
	
	strcpy(nahida.name, "Nahida"); // 1.1k atk, 951 em | wandering evenstar, 4p dwm, EM/EM/Crit, 1/9/9
	nahida.HP = 10360 * (1 + 0.0992/100) + 4780 + 507.88; // base + substat
	nahida.ATK = (299 + 510) * (1 + 0.0992/100.0) + 311 + 33.08; // base + sig + substat
	nahida.ER = 1 + 0.2204;
	nahida.EM = 115.2 + 165 + 187 + 187 + 187 + 50 + 59.46; // ascension + wandering evenstar + em sands + em gob + dendro resonance + substat
	nahida.DMGBonus = ((nahida.EM - 200) * 0.1/100) < 80.0/100 ? ((nahida.EM - 200) * 0.1/100) : 80.0/100; // a4 passive 
	nahida.DMGBonus += 15/100.0; // 2p dwm
	nahida.critRate = ((nahida.EM - 200) * 0.03/100) < 24.0/100 ? ((nahida.EM - 200) * 0.03/100) : 24.0/100; // a4 passive
	nahida.critRate += 5/100.0 + 0.2979; // base + substat
	nahida.critDMG = 50/100.0 + 0.7944; // base + substat
	nahida.currHP = 100.0/100.0;
	nahida.HB = 0.0/100.0;
	
	strcpy(baizhu.name, "Baizhu"); // 46k hp | prototype amber, 4p ohc, HP/HP/HP, 1/9/9
	baizhu.HP = 13380 * (1 + (28.8 + 46.6 + 46.6 + 46.6 + 41.3 + 0.2976)/100) + 4780 + 507.88; // base + ascension + hp sands + hp gob + proto amber + substat
	baizhu.ATK = (193 + 510) * (1 + 0.0992) + 311 + 33.08; // base + fav codex + substat
	baizhu.ER = 1 + 0.3857;
	baizhu.EM = 39.64 + 50; // substat + dendro resonance
	baizhu.DMGBonus = 25/100.0; // a1 passive
	baizhu.critRate = 5/100.0 + 0.2648; // base + substat
	baizhu.critDMG = 50/100.0 + 0.4634; // base + substat
	baizhu.currHP = 100.0/100.0;
	baizhu.HB = 15.0/100; // 2p ohc + hb circlet
	baizhu.incomingHB = 0.0/100.0;
	
	strcpy(furina.name, "Furina"); // 37k hp, 161.5 cv | fav sword, 4p gt, HP/HP/Crit, 1/9/9
	furina.HP = 15307 * (1 + 46.6/100 + 46.6/100 + 0.1984) + 4780 + 507.88; // base + hp sands + hp gob + substat
	furina.ER = 1 + 0.1102 + 61.3/100;
	furina.EM = 39.64 + 50; // substat + dendro resonance
	furina.DMGBonus = (0.7/100 * furina.HP/1000) < 28.0/100 ? (0.7/100 * furina.HP/1000) : 28.0/100; // a4 passive
	furina.DMGBonus += 70/100.0; // 4p gt
	furina.critRate = 5/100.0 + 19.2/100 + 0.3972; // base + ascension + substat
	furina.critDMG = 50/100.0 + 62.2/100 + 0.662; // base + cd circlet + substat
	furina.currHP = 100.0/100.0;
	furina.HB = 0.0/100.0;
	furina.incomingHB = 0.0/100.0;
}

void viewData(){
	printf("%s HP: %.0f\n", cyno.name, cyno.HP);
	printf("%s ATK: %.0f\n", cyno.name, cyno.ATK);
	printf("%s EM: %.0f\n", cyno.name, cyno.EM);
	printf("%s DMG Bonus: %.1f%%\n", cyno.name, cyno.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", cyno.name, cyno.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", cyno.name, cyno.critDMG * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", cyno.name, cyno.incomingHB * 100);
	
	puts("");
	
	printf("%s HP: %.0f\n", nahida.name, nahida.HP);
	printf("%s ATK: %.0f\n", nahida.name, nahida.ATK);
	printf("%s EM: %.0f\n", nahida.name, nahida.EM);
	printf("%s DMG Bonus: %.1f%%\n", nahida.name, nahida.DMGBonus * 100);
	printf("%s CRIT Rate: %.1f%%\n", nahida.name, nahida.critRate * 100);
	printf("%s CRIT DMG: %.1f%%\n", nahida.name, nahida.critDMG * 100);
	printf("%s Incoming Healing Bonus: %.1f%%\n", nahida.name, nahida.incomingHB * 100);
	
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

void furinaSeahorse(){
	furina.damage += (5.49/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	totalDamage += (5.49/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	cyno.prevHP = cyno.currHP;
	nahida.prevHP = nahida.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	cyno.currHP -= 1.6/100;
	nahida.currHP -= 1.6/100;
	baizhu.currHP -= 1.6/100;
	furina.currHP -= 1.6/100;
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 4 * 1.6;
	
	if(fanfare > 300) fanfare = 300;
	
	cyno.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	nahida.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	cyno.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	nahida.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void furinaOctopus(){
	furina.damage += (10.13/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	totalDamage += (10.13/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	cyno.prevHP = cyno.currHP;
	nahida.prevHP = nahida.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	cyno.currHP -= 2.4/100;
	nahida.currHP -= 2.4/100;
	baizhu.currHP -= 2.4/100;
	furina.currHP -= 2.4/100;
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 4 * 2.4;
	
	if(fanfare > 300) fanfare = 300;
	
	cyno.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	nahida.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	cyno.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	nahida.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void furinaCrab(){
	furina.damage += (14.09/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	totalDamage += (14.09/100 * furina.HP) * (1 + furina.critDMG * furina.critRate) * (1 + furina.DMGBonus) * 140.0/100 * enemyRes * enemyDef;
	cyno.prevHP = cyno.currHP;
	nahida.prevHP = nahida.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	cyno.currHP -= 3.6/100;
	nahida.currHP -= 3.6/100;
	baizhu.currHP -= 3.6/100;
	furina.currHP -= 3.6/100;
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 4 * 3.6;
	
	if(fanfare > 300) fanfare = 300;
	
	cyno.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	nahida.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	cyno.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	nahida.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void nahidaTriKarmaPurification(double spread){
	nahida.damage += ((185.8/100 * nahida.ATK) + (371.5/100 * nahida.EM) + spread + nahidaWanderingEvenstarBuff) * (1 + nahida.critRate * nahida.critDMG) * (1 + nahida.DMGBonus) * dendroRes * enemyDef;
	totalDamage += ((185.8/100 * nahida.ATK) + (371.5/100 * nahida.EM) + spread + nahidaWanderingEvenstarBuff) * (1 + nahida.critRate * nahida.critDMG) * (1 + nahida.DMGBonus) * dendroRes * enemyDef;
}

void cynoSkill(double agg, double endseer){
	cyno.damage += (266.6/100 * cyno.ATK + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus + endseer) * enemyRes * enemyDef;
	totalDamage += (266.6/100 * cyno.ATK + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus + endseer) * enemyRes * enemyDef;
}

void cynoHyperbloom(){
	cyno.damage += 3 * 1446.85 * (1 + (16.0 * cyno.EM)/(2000.0 + cyno.EM) + 40.0/100 + baizhuBloomBuff) * dendroRes;
	totalDamage += 3 * 1446.85 * (1 + (16.0 * cyno.EM)/(2000.0 + cyno.EM) + 40.0/100 + baizhuBloomBuff) * dendroRes;
}

void cynoN1(double agg){
	cyno.damage += (143.8/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (143.8/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
}

void cynoN2(double agg){
	cyno.damage += (151.5/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (151.5/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
}

void cynoN3(double agg){
	cyno.damage += (192.2/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (192.2/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
}

void cynoN4a(double agg){
	cyno.damage += (95.0/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (95.0/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
}

void cynoN4b(double agg){
	cyno.damage += (95.0/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (95.0/100 * cyno.ATK + 150.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
}

void cynoA1(double agg){
	cyno.damage += (100.0/100 * cyno.ATK + 250.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (100.0/100 * cyno.ATK + 250.0/100 * cyno.EM + agg + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
}

void baizhuOHC(){
	baizhuOHCHeal > 30000 ? baizhuOHCHeal = 30000 : baizhuOHCHeal = baizhuOHCHeal;
	baizhu.damage += baizhuOHCHeal * 0.9 * enemyRes;
	totalDamage += baizhuOHCHeal * 0.9 * enemyRes;
}

void baizhuSkillHeal(){
	cyno.prevHP = cyno.currHP;
	nahida.prevHP = nahida.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	cyno.currHP += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + cyno.incomingHB) / cyno.HP;
	nahida.currHP += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + nahida.incomingHB) / nahida.HP;
	baizhu.currHP += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + baizhu.incomingHB) / baizhu.HP;
	furina.currHP += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + furina.incomingHB) / furina.HP;
	baizhuOHCHeal += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + cyno.incomingHB);
	baizhuOHCHeal += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + nahida.incomingHB);
	baizhuOHCHeal += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + baizhu.incomingHB);
	baizhuOHCHeal += (13.6/100 * baizhu.HP + 1566) * (1 + baizhu.HB + furina.incomingHB);
	if(cyno.currHP > 1) cyno.currHP = 1;
	if(nahida.currHP > 1) nahida.currHP = 1;
	if(baizhu.currHP > 1) baizhu.currHP = 1;
	if(furina.currHP > 1) furina.currHP = 1;
	prevFanfare = fanfare;
	fanfare += (cyno.currHP - cyno.prevHP) * 100;
	fanfare += (nahida.currHP - nahida.prevHP) * 100;
	fanfare += (baizhu.currHP - baizhu.prevHP) * 100;
	fanfare += (furina.currHP - furina.prevHP) * 100;
	
	cyno.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	nahida.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	cyno.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	nahida.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void baizhuBurstHeal(){
	cyno.prevHP = cyno.currHP;
	nahida.prevHP = nahida.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	
	switch (activeChar){
		case 1:
			cyno.currHP += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + cyno.incomingHB) / cyno.HP;
			baizhuOHCHeal += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + cyno.incomingHB);
			if(cyno.currHP > 1) cyno.currHP = 1;
			break;
		case 2:
			nahida.currHP += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + nahida.incomingHB) / nahida.HP;
			baizhuOHCHeal += (8.84/100 * baizhu.HP + 1018) * (1 + baizhu.HB + nahida.incomingHB);
			if(nahida.currHP > 1) nahida.currHP = 1;
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
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (cyno.currHP - cyno.prevHP);
	fanfare += 100 * (nahida.currHP - nahida.prevHP);
	fanfare += 100 * (baizhu.currHP - baizhu.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	cyno.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	nahida.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	cyno.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	nahida.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
}

void baizhuProtoHeal(){
	cyno.prevHP = cyno.currHP;
	nahida.prevHP = nahida.currHP;
	baizhu.prevHP = baizhu.currHP;
	furina.prevHP = furina.currHP;
	
	cyno.currHP += 6.0/100 * (1 + cyno.incomingHB);
	if(cyno.currHP > 1) cyno.currHP = 1;
	nahida.currHP += 6.0/100 * (1 + nahida.incomingHB);
	if(nahida.currHP > 1) nahida.currHP = 1;
	baizhu.currHP += 6.0/100 * (1 + baizhu.incomingHB);
	if(baizhu.currHP > 1) baizhu.currHP = 1;
	furina.currHP += 6.0/100 * (1 + furina.incomingHB);
	if(furina.currHP > 1) furina.currHP = 1;
	
	if(!furinaBurstActive) return;
	
	prevFanfare = fanfare;
	fanfare += 100 * (cyno.currHP - cyno.prevHP);
	fanfare += 100 * (nahida.currHP - nahida.prevHP);
	fanfare += 100 * (baizhu.currHP - baizhu.prevHP);
	fanfare += 100 * (furina.currHP - furina.prevHP);
	
	if(fanfare > 300) fanfare = 300;
	
	cyno.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	nahida.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	baizhu.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	furina.DMGBonus += (fanfare - prevFanfare) * 0.23/100;
	
	cyno.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	nahida.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	baizhu.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	furina.incomingHB += (fanfare - prevFanfare) * 0.09/100;
	
	baizhuBloomBuff = baizhu.HP/1000 * 2.0/100;
	baizhuAdditiveBuff = baizhu.HP/1000 * 0.8/100;
}

void baizhuBurstDamage(){
	baizhu.damage += (165.0/100 * baizhu.ATK + alliesWanderingEvenstarBuff) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	totalDamage += (165.0/100 * baizhu.ATK + alliesWanderingEvenstarBuff) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
}




double prevTime = 0.0;
double time = 0.0;
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
						 
						 
int blooms;
int aggravates;
int spreads;

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
				// Electro Aura + Anemo = Electro Swirl
				case 1:
					auraGauge -= 0.5 * applyGauge;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Swirled Electro. %s Aura remains at %.2fs\n", elements[auraType], time);
						break;
					}
					
					printf("Swirled Electro. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
				// Electro Aura + Geo = Electro Crystallize
				case 2:
					auraGauge -= 0.5 * applyGauge;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Crystallized Electro. %s Aura remains at %.2fs\n", elements[auraType], time);
						break;
					}
					
					printf("Crystallized Electro. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
				
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
					
				// Electro Aura + Pyro = Overloaded
				case 6:
					auraGauge -= applyGauge;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Triggered Overloaded. %s Aura remains at %.2fs\n", elements[auraType], time);
						break;
					}
					
					printf("Triggered Overloaded. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
				// Electro Aura + Cryo = Superconduct
				case 7:
					auraGauge -= applyGauge;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Triggered Superconduct. %s Aura remains at %.2fs\n", elements[auraType], time);
						break;
					}
					
					printf("Triggered Superconduct. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
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
						printf("Bloom created. %s Aura remains at %.2fs\n", elements[auraType], time);
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
				// Hydro Aura + Anemo = Hydro Swirl
				case 1:
					auraGauge -= 0.5 * applyGauge;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Swirled Hydro. %s Aura remains at %.2fs\n", elements[auraType], time);
						break;
					}
					
					printf("Swirled Hydro. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
				// Hydro Aura + Geo = Hydro Crystallize
				case 2:
					auraGauge -= 0.5 * applyGauge;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Crystallized Hydro. %s Aura remains at %.2fs\n", elements[auraType], time);
						break;
					}
					
					printf("Crystallized Hydro. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
					break;
					
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
					
				// Hydro Aura + Pyro = Reverse Vaporize
				case 6:
					auraGauge -= 0.5 * applyGauge;
					if(auraGauge <= 0){
						auraType = 8;
						auraGauge = 0.0;
						printf("Triggered Vaporize. %s Aura remains at %.2fs\n", elements[auraType], time);
						break;
					}
					
					printf("Triggered Vaporize. %s Aura of %.3fU remain at %.2fs\n", elements[auraType], auraGauge, time);
					
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
	
	//0.00 update buff
	nahidaWanderingEvenstarBuff = 24.0/100 * nahida.EM;
	alliesWanderingEvenstarBuff = 8.0/100 * nahida.EM;
	
	//0.28 furina skill
	activeChar = 4;
	
	//0.83 nahida hold skill
	puts("Nahida");
	activeChar = 2;
	addReaction(0.83, 4, 1.0);
	
	nahida.damage += (234.7/100 * nahida.ATK + nahidaWanderingEvenstarBuff) * (1 + nahida.critRate * nahida.critDMG) * (1 + 0.35) * dendroRes * enemyDef;
	totalDamage += (234.7/100 * nahida.ATK + nahidaWanderingEvenstarBuff) * (1 + nahida.critRate * nahida.critDMG) * (1 + 0.35) * dendroRes * enemyDef;
	
	dendroRes = 1.15;
	// dendro reso buff
	cyno.EM += 30;
	nahida.EM += 30;
	baizhu.EM += 30;
	furina.EM += 30;
	
	//1.15 nahida burst (cast)
	
	//2.27 nahida burst (finish)
	nahidaEMBuff = nahida.EM * 0.25;
	if(nahidaEMBuff > 250) nahidaEMBuff = 250;
	// ends on 22.95s
	nahida.EM += nahidaEMBuff;
	
	//1.88 furina seahorse
	puts("Furina");
	addReaction(1.88, 5, 1.0);
	furinaSeahorse();
	
	//2.37 furina octopus, crab
	puts("Furina");
	addReaction(2.37, 5, 1.0);
	furinaOctopus();
	
	puts("Furina");
	addReaction(2.37, 5, 1.0);
	furinaCrab();
	
	//3.33 furina burst (cast)
	activeChar = 4;
	nahida.EM -= nahidaEMBuff;
	furina.EM += nahidaEMBuff;
	
	//3.51 furina seahorse
	furinaSeahorse();
	
	//4.75 furina burst
	puts("Furina");
	addReaction(4.75, 5, 1.0);
	furina.damage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus - 70.0/100) * enemyRes * enemyDef;
	totalDamage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus - 70.0/100) * enemyRes * enemyDef;
	furinaBurstActive = true;
	prevFanfare = 0;
	fanfare = 0;
	
	//5.22 furina seahorse
	puts("Furina");
	addReaction(5.22, 5, 1.0);
	furinaSeahorse();
	
	//5.45 swap to baizhu
	activeChar = 3;
	furina.EM -= nahidaEMBuff;
	baizhu.EM += nahidaEMBuff;
	
	//5.60 furina octopus
	furinaOctopus();
	
	//5.65 baizhu skill
	puts("Baizhu");
	addReaction(5.65, 4, 1.0);
	baizhu.damage += 3 * (134.6/100 * baizhu.ATK + alliesWanderingEvenstarBuff) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	totalDamage += 3 * (134.6/100 * baizhu.ATK + alliesWanderingEvenstarBuff) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	
	//5.72 nahida tkp
	puts("Nahida");
	addReaction(5.72, 4, 1.5);
	nahidaTriKarmaPurification(0);
	
	//5.97 baizhu burst (cast)
	
	//6.82 furina seahorse
	furinaSeahorse();
	
	//7.50 furina crab
	puts("Furina");
	addReaction(7.50, 5, 1.0);
	furinaCrab();
	
	//7.83 baizhu burst (finish)
	
	//8.35 cyno skill
	activeChar = 1;
	baizhu.EM -= nahidaEMBuff;
	cyno.EM += nahidaEMBuff;
	
	baizhuSkillHeal();
	
	puts("Cyno");
	addReaction(8.35, 3, 1.0);
	cyno.damage += (234.7/100 * cyno.ATK + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (234.7/100 * cyno.ATK + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
//	cynoStaffStacks++;
	
	//hyperbloom
	cynoHyperbloom();
	cynoHyperbloom();
	
	cyno.EM += 20;
	nahida.EM += 20;
	baizhu.EM += 20;
	furina.EM += 20;
	
	//8.42 nahida tkp
	puts("Nahida");
	addReaction(8.42, 4, 1.5);	
	nahidaSpread = 1.25 * 1446.85 * (1 + (5 * nahida.EM)/(1200 + nahida.EM));
	nahidaTriKarmaPurification(nahidaSpread);
	
	//8.53 furina seahorse
	puts("Furina");
	addReaction(8.53, 5, 1.0);
	furinaSeahorse();
	
	//8.75 cyno burst (cast)
	
	//8.91 furina octopus
	puts("Furina");
	addReaction(8.91, 5, 1.0);
	furinaOctopus();
	
	//9.33 baizhu burst heal
	baizhuBurstHeal();
	
	//9.83 baizhu proto heal
	baizhuProtoHeal();
	
	//10.00 update buff
	nahidaWanderingEvenstarBuff = 24.0/100 * nahida.EM;
	alliesWanderingEvenstarBuff = 8.0/100 * nahida.EM;
	
	//10.12 furina seahorse
	furinaSeahorse();
	
	//10.17 baizhu burst damage
	puts("Baizhu");
	addReaction(10.17, 4, 1.0);
	baizhuBurstDamage();
	
	//10.78 cyno skill
	puts("Cyno");
	addReaction(10.78, 3, 1.0);
	cynoAgg = 1.15 * 1446.85 * (1 + (5 * cyno.EM)/(1200 + cyno.EM) + 20.0/100 + baizhuAdditiveBuff);
	cyno.EM += 100;
	cynoSkill(cynoAgg, 0);
//	cynoStaffStacks++;
	cynoHyperbloom();
	cynoHyperbloom();
	
	//10.82 nahida tkp
	puts("Nahida");
	addReaction(10.82, 4, 1.5);
	nahidaSpread = 1.25 * 1446.85 * (1 + (5 * nahida.EM)/(1200 + nahida.EM));
	nahidaTriKarmaPurification(nahidaSpread);
	
	//11.07 cyno n1
	puts("Cyno");
	addReaction(11.07, 3, 1.0);
	cynoAgg = 1.15 * 1446.85 * (1 + (5 * cyno.EM)/(1200 + cyno.EM) + 20.0/100 + baizhuAdditiveBuff);
	cynoN1(cynoAgg);
	
	//11.35 baizhu ohc pop
	baizhuOHC();
	
	//11.38 cyno n2
	cynoN2(0);
	
	//11.65 furina seahorse
	puts("Furina");
	addReaction(11.65, 5, 1.0);
	furinaSeahorse();
	
	//11.83 baizhu burst heal
	baizhuBurstHeal();
	
	//11.83 baizhu proto heal
	baizhuProtoHeal();
	
	//11.83 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//11.98 cyno n3
	cynoN3(0);
	
	//12.17 furina octopus
	furinaOctopus();
	
	//12.47 cyno n4a
	puts("Cyno");
	addReaction(12.47, 3, 1.0);
	cynoN4a(cynoAgg);
	cynoHyperbloom();
	
	//12.60 baizhu burst damage
	puts("Baizhu");
	addReaction(12.60, 4, 1.0);
	baizhuBurstDamage();
	
	//12.63 cyno n4b
	cynoN4b(0);
	
	//12.68 furina crab
	puts("Furina");
	addReaction(12.68, 5, 1.0);
	furinaCrab();
	
	//13.18 cyno skill
	puts("Cyno");
	addReaction(13.18, 3, 1.0);
	cynoSkill(cynoAgg, 0);
//	cynoStaffStacks++;
	cynoHyperbloom();
	
	//13.25 nahida tkp
	puts("Nahida");
	addReaction(13.25, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//13.29 furina seahorse
	furinaSeahorse();
	
	//13.50 cyno n1
	cynoN1(0);
	
	//13.82 cyno n2
	puts("Cyno");
	addReaction(13.82, 3, 1.0);
	cynoN2(cynoAgg);
	
	//13.83 baizhu proto heal
	baizhuProtoHeal();
	
	//14.33 baizhu burst heal
	baizhuBurstHeal();
	
	//14.42 cyno n3
	cynoN3(0);
	
	//14.85 baizhu ohc pop
	baizhuOHC();
	
	//14.89 furina seahorse
	furinaSeahorse();
	puts("Furina");
	addReaction(14.89, 5, 1.0);
	
	//14.90 cyno n4a
	cynoN4a(0);
	
	//15.03 baizhu burst damage
	puts("Baizhu");
	addReaction(15.03, 4, 1.0);
	baizhuBurstDamage();
	
	//15.07 cyno n4b
	puts("Cyno");
	addReaction(15.07, 3, 1.0);
	cynoN4b(cynoAgg);
	cynoHyperbloom();
	
	//15.35 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//15.37 furina octopus
	puts("Furina");
	addReaction(15.37, 5, 1.0);
	furinaOctopus();
	
	//15.62 cyno skill
	puts("Cyno");
	addReaction(15.62, 3, 1.0);
	cynoSkill(cynoAgg, 0.35);
	cynoHyperbloom();
	addReaction(15.62, 3, 1.0);
	cynoA1(cynoAgg);
	cynoA1(0);
	cynoA1(0);
	
	//15.68 nahida tkp
	puts("Nahida");
	addReaction(15.68, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//15.95 cyno n1
	cynoN1(0);
	
	//16.27 cyno n2
	cynoN2(0);
	
	//16.48 furina seahorse
	furinaSeahorse();
	
	//16.83 baizhu burst heal
	baizhuBurstHeal();
	
	//16.87 cyno n3
	puts("Cyno");
	addReaction(16.87, 3, 1.0);
	cynoN3(cynoAgg);
	
	//17.35 cyno n4a
	cynoN4a(0);
	
	//17.47 baizhu burst damage
	puts("Baizhu");
	addReaction(17.47, 4, 1.0);
	baizhuBurstDamage();
	
	//17.52 cyno n4b
	cynoN4b(0);
	
	//17.92 furina crab
	puts("Furina");
	addReaction(17.92, 5, 1.0);
	furinaCrab();
	
	//18.07 cyno skill
	puts("Cyno");
	addReaction(18.07, 3, 1.0);
	cynoSkill(cynoAgg, 0);
	cynoHyperbloom();
	
	//18.12 furina seahorse
	puts("Furina");
	addReaction(18.12, 5, 1.0);
	furinaSeahorse();
	
	//18.13 nahida tkp
	puts("Nahida");
	addReaction(18.13, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//18.35 baizhu ohc pop
	baizhuOHC();
	
	//18.38 cyno n1
	puts("Cyno");
	addReaction(18.38, 3, 1.0);
	cynoN1(cynoAgg);
	cynoHyperbloom();
	cynoHyperbloom();
	
	//18.53 furina octopus
	furinaOctopus();
	
	//18.70 cyno n2
	cynoN2(0);
	
	//18.85 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//19.30 cyno n3
	cynoN3(0);
	
	//19.33 baizhu burst heal
	baizhuBurstHeal();
	
	//19.73 furina seahorse
	furinaSeahorse();
	
	//19.78 cyno n4a
	puts("Cyno");
	addReaction(19.78, 3, 1.0);
	cynoN4a(cynoAgg);
	
	//19.90 baizhu burst damage
	puts("Baizhu");
	addReaction(19.90, 4, 1.0);
	baizhuBurstDamage();
	
	//19.95 cyno n4b
	cynoN4b(0);
	
	//20.00 update buff
	nahidaWanderingEvenstarBuff = 24.0/100 * nahida.EM;
	alliesWanderingEvenstarBuff = 8.0/100 * nahida.EM;
	
	//20.50 cyno skill
	puts("Cyno");
	addReaction(20.50, 3, 1.0);
	cynoSkill(cynoAgg, 0);
	
	//20.57 nahida tkp
	puts("Nahida");
	addReaction(20.57, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//20.83 cyno n1
	cynoN1(0);
	
	//21.15 cyno n2	
	puts("Cyno");
	addReaction(21.15, 3, 1.0);
	cynoN2(cynoAgg);
	
	//21.25 furina seahorse
	puts("Furina");
	addReaction(21.25, 5, 1.0);
	furinaSeahorse();
	
	//21.75 cyno n3
	cynoN3(0);
	cynoHyperbloom();
	
	//21.76 furina octopus
	puts("Furina");
	addReaction(21.76, 5, 1.0);
	furinaOctopus();
	
	//21.83 baizhu burst heal
	baizhuBurstHeal();

	//21.85 baizhu ohc pop
	baizhuOHC();
	
	//22.23 cyno n4a
	cynoN4a(0);
	cynoHyperbloom();
	
	//22.35 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//22.40 cyno n4b
	puts("Cyno");
	addReaction(22.40, 3, 1.0);
	cynoN4b(cynoAgg);
	cynoHyperbloom();
	
	//22.43 baizhu burst damage
	puts("Baizhu");
	addReaction(22.43, 4, 1.0);
	baizhuBurstDamage();
	
	//22.75 furina burst expires
	furinaBurstActive = false;
	
	cyno.DMGBonus -= (fanfare) * 0.23/100;
	nahida.DMGBonus -= (fanfare) * 0.23/100;
	baizhu.DMGBonus -= (fanfare) * 0.23/100;
	furina.DMGBonus -= (fanfare) * 0.23/100;
	
	cyno.incomingHB -= (fanfare) * 0.09/100;
	nahida.incomingHB -= (fanfare) * 0.09/100;
	baizhu.incomingHB -= (fanfare) * 0.09/100;
	furina.incomingHB -= (fanfare) * 0.09/100;
	
	fanfare = 0;
	prevFanfare = 0;
	
	//22.92 furina seahorse
	furinaSeahorse();
	
	//22.95 nahida burst expires
	cyno.EM -= nahidaEMBuff;
	
	//22.95 cyno skill
	puts("Cyno");
	addReaction(22.95, 3, 1.0);
	cynoSkill(cynoAgg, 0);
	cynoHyperbloom();
	
	//23.02 nahida tkp
	puts("Nahida");
	addReaction(23.02, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	
//	printf("Number of blooms: %d\n", blooms);
//	printf("Number of aggravates: %d\n", aggravates);
//	printf("Number of spreads: %d\n", spreads);
	
	puts("");
	
	viewData();
	
	puts("");
	
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", cyno.name, cyno.currHP * 100, cyno.currHP * cyno.HP, cyno.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", nahida.name, nahida.currHP * 100, nahida.currHP * nahida.HP, nahida.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", baizhu.name, baizhu.currHP * 100, baizhu.currHP * baizhu.HP, baizhu.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", furina.name, furina.currHP * 100, furina.currHP * furina.HP, furina.HP);
	
	puts("");
	
	printf("Fanfare: %.1f\n", fanfare);
	printf("Total Damage: %.1f\n", totalDamage);
	printf("Cyno Damage: %.1f\n", cyno.damage);
	printf("Nahida Damage: %.1f\n", nahida.damage);
	printf("Baizhu Damage: %.1f\n", baizhu.damage);
	printf("Furina Damage: %.1f\n", furina.damage);
	
	puts("");
	printf("Damage per Second: %.1f\n", totalDamage / 23.0);
	printf("Cyno DPS: %.1f\n", cyno.damage / 23.0);
	printf("Nahida DPS: %.1f\n", nahida.damage / 23.0);
	printf("Baizhu DPS: %.1f\n", baizhu.damage / 23.0);
	printf("Furina DPS: %.1f\n", furina.damage / 23.0);
	
	puts("");
	printf("Cyno Contribution: %.1f%%\n", cyno.damage / totalDamage * 100);
	printf("Nahida Contribution: %.1f%%\n", nahida.damage / totalDamage * 100);
	printf("Baizhu Contribution: %.1f%%\n", baizhu.damage / totalDamage * 100);
	printf("Furina Contribution: %.1f%%\n", furina.damage / totalDamage * 100);
	
	puts("");
	printf("Aggravates: %d, Spreads: %d, Blooms: %d\n", aggravates, spreads, blooms);
	aggravates = 0;
	spreads = 0;
	blooms = 0;
	
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
	
	// 2nd rot
	
	puts("~~~~~~~~~~~~~~~~~~~");
	puts("| SECOND ROTATION |");
	puts("~~~~~~~~~~~~~~~~~~~");
	
	totalDamage = 0.0;
	cyno.damage = 0.0;
	nahida.damage = 0.0;
	baizhu.damage = 0.0;
	furina.damage = 0.0;
	
	//23.11 furina crab
	puts("Furina");
	addReaction(23.11, 5, 1.0);
	furinaCrab();
	
	//23.47 nahida hold skill
	puts("Nahida");
	activeChar = 2;
	addReaction(23.47, 4, 1.0);
	
	nahida.damage += (234.7/100 * nahida.ATK + nahidaWanderingEvenstarBuff) * (1 + nahida.critRate * nahida.critDMG) * (1 + 0.35) * dendroRes * enemyDef;
	totalDamage += (234.7/100 * nahida.ATK + nahidaWanderingEvenstarBuff) * (1 + nahida.critRate * nahida.critDMG) * (1 + 0.35) * dendroRes * enemyDef;
	
	dendroRes = 1.15;
	
	//24.00 nahida burst (cast)
	
	//24.41 furina seahorse
	puts("Furina");
	addReaction(24.41, 5, 1.0);
	furinaSeahorse();
	
	//25.12 furina octopus
	furinaOctopus();
	
	//25.60 nahida burst (finish)
	nahidaEMBuff = nahida.EM * 0.25;
	if(nahidaEMBuff > 250) nahidaEMBuff = 250;
	// ends on 46.60s
	nahida.EM += nahidaEMBuff;
	
	//26.18 furina skill
	activeChar = 4;
	nahida.EM -= nahidaEMBuff;
	furina.EM += nahidaEMBuff;
	
	//26.55 furina burst (cast)
	
	//27.78 furina seahorse
	furinaSeahorse();
	
	//28.07 furina burst
	puts("Furina");
	addReaction(28.07, 5, 1.0);
	furina.damage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus - 70.0/100) * enemyRes * enemyDef;
	totalDamage += (19.4/100 * furina.HP) * (1 + furina.critRate * furina.critDMG) * (1 + furina.DMGBonus - 70.0/100) * enemyRes * enemyDef;
	furinaBurstActive = true;
	prevFanfare = 0;
	fanfare = 0;
	
	//28.27 furina octopus
	puts("Furina");
	addReaction(28.27, 5, 1.0);
	furinaOctopus();
	
	//28.27 furina crab
	puts("Furina");
	addReaction(28.27, 5, 1.0);
	furinaCrab();
	
	//28.67 swap to baizhu
	activeChar = 3;
	furina.EM -= nahidaEMBuff;
	baizhu.EM += nahidaEMBuff;
	
	//28.88 baizhu skill
	puts("Baizhu");
	addReaction(28.88, 4, 1.0);
	baizhu.damage += 3 * (134.6/100 * baizhu.ATK + alliesWanderingEvenstarBuff) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	totalDamage += 3 * (134.6/100 * baizhu.ATK + alliesWanderingEvenstarBuff) * (1 + baizhu.critRate * baizhu.critDMG) * (1 + baizhu.DMGBonus) * dendroRes * enemyDef;
	
	//29.22 baizhu burst (cast)
	
	//29.41 furina seahorse
	furinaSeahorse();
	
	//30.00 update buff
	nahidaWanderingEvenstarBuff = 24.0/100 * nahida.EM;
	alliesWanderingEvenstarBuff = 8.0/100 * nahida.EM;
	
	//31.08 baizhu burst (finish)
	
	//31.12 furina seahorse
	puts("Furina");
	addReaction(31.12, 5, 1.0);
	furinaSeahorse();
	
	//31.50 furina octopus
	furinaOctopus();
	
	//31.60 cyno skill
	activeChar = 1;
	baizhu.EM -= nahidaEMBuff;
	cyno.EM += nahidaEMBuff;
	
	baizhuSkillHeal();
	
	puts("Cyno");
	addReaction(31.60, 3, 1.0);
	cyno.damage += (234.7/100 * cyno.ATK + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
	totalDamage += (234.7/100 * cyno.ATK + alliesWanderingEvenstarBuff) * (1 + cyno.critDMG * cyno.critRate) * (1 + cyno.DMGBonus) * enemyRes * enemyDef;
//	cynoStaffStacks++;
	cyno.damage += 3 * 1446.85 * (1 + (16.0 * cyno.EM)/(2000.0 + cyno.EM) + 40.0/100) * dendroRes;
	totalDamage += 3 * 1446.85 * (1 + (16.0 * cyno.EM)/(2000.0 + cyno.EM) + 40.0/100) * dendroRes;
	
	cyno.damage += 3 * 1446.85 * (1 + (16.0 * cyno.EM)/(2000.0 + cyno.EM) + 40.0/100) * dendroRes;
	totalDamage += 3 * 1446.85 * (1 + (16.0 * cyno.EM)/(2000.0 + cyno.EM) + 40.0/100) * dendroRes;
	
	//31.97 cyno burst (cast)
	
	//32.58 baizhu burst heal
	baizhuBurstHeal();
	
	//32.67 nahida tkp
	puts("Nahida");
	addReaction(32.67, 4, 1.5);	
	nahidaSpread = 1.25 * 1446.85 * (1 + (5 * nahida.EM)/(1200 + nahida.EM));
	nahidaTriKarmaPurification(nahidaSpread);
	
	//32.72 furina seahorse
	furinaSeahorse();
	
	//33.08 baizhu proto heal
	baizhuProtoHeal();
	
	//33.40 furina crab
	puts("Furina");
	addReaction(33.40, 5, 1.0);
	furinaCrab();
	
	//33.42 baizhu burst damage
	puts("Baizhu");
	addReaction(33.42, 4, 1.0);
	baizhuBurstDamage();
	
	//33.53 cyno skill
	puts("Cyno");
	addReaction(33.53, 3, 1.0);
	cynoAgg = 1.15 * 1446.85 * (1 + (5 * cyno.EM)/(1200 + cyno.EM) + 20.0/100 + baizhuAdditiveBuff);
	cyno.EM += 100;
	cynoSkill(cynoAgg, 0);
//	cynoStaffStacks++;
	cynoHyperbloom();
	cynoHyperbloom();
	
	//33.55 baizhu ohc pop
	baizhuOHC();
	
	//34.05 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//34.43 furina seahorse
	puts("Furina");
	addReaction(34.43, 5, 1.0);
	furinaSeahorse();
	
	//35.08 baizhu burst heal
	baizhuBurstHeal();
	
	//35.08 baizhu proto heal
	baizhuProtoHeal();
	
	//34.33 cyno n1
	puts("Cyno");
	addReaction(34.33, 3, 1.0);
	cynoAgg = 1.15 * 1446.85 * (1 + (5 * cyno.EM)/(1200 + cyno.EM) + 20.0/100 + baizhuAdditiveBuff);
	cynoN1(cynoAgg);
	cynoHyperbloom();
	
	//34.65 cyno n2
	cynoN2(0);
	
	//34.81 furina octopus
	puts("Furina");
	addReaction(34.81, 5, 1.0);
	furinaOctopus();
	
	//35.25 cyno n3
	cynoN3(0);
	cynoHyperbloom();
	
	//35.73 cyno n4a
	puts("Cyno");
	addReaction(35.73, 3, 1.0);
	cynoN4a(cynoAgg);
	
	//35.80 nahida tkp
	puts("Nahida");
	addReaction(35.80, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//35.85 baizhu burst damage
	puts("Baizhu");
	addReaction(35.85, 4, 1.0);
	baizhuBurstDamage();
	
	//35.90 cyno n4b
	cynoN4b(0);
	cynoHyperbloom();
	
	//36.02 furina seahorse
	furinaSeahorse();
	
	//36.47 cyno skill
	puts("Cyno");
	addReaction(36.47, 3, 1.0);
	cynoSkill(cynoAgg, 0);
//	cynoStaffStacks++;
	
	//36.78 cyno n1
	cynoN1(0);
	
	//37.08 baizhu proto heal
	baizhuProtoHeal();
	
	//37.10 cyno n2
	puts("Cyno");
	addReaction(37.10, 3, 1.0);
	cynoN2(cynoAgg);
	
	//37.28 baizhu burst heal
	baizhuBurstHeal();
	
	//37.55 furina seahorse
	puts("Furina");
	addReaction(37.55, 5, 1.0);
	furinaSeahorse();
	
	//37.70 cyno n3
	cynoN3(0);
	cynoHyperbloom();
	
	//38.07 furina octopus
	furinaOctopus();
	
	//38.18 cyno n4a
	cynoN4a(0);
	
	//38.23 baizhu ohc pop
	baizhuOHC();
	
	//38.28 baizhu burst damage
	puts("Baizhu");
	addReaction(38.28, 4, 1.0);
	baizhuBurstDamage();
	
	//38.35 cyno n4b
	puts("Cyno");
	addReaction(38.35, 3, 1.0);
	cynoN4b(cynoAgg);
	
	//38.35 nahida tkp
	puts("Nahida");
	addReaction(38.35, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//38.58 furina crab
	puts("Furina");
	addReaction(38.58, 5, 1.0);
	furinaCrab();
	
	//38.73 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//38.90 cyno skill
	puts("Cyno");
	addReaction(38.90, 3, 1.0);
	cynoSkill(cynoAgg, 0.35);
	addReaction(38.90, 3, 1.0);
	cynoA1(cynoAgg);
	cynoA1(0);
	cynoA1(0);
	cynoHyperbloom();
	
	//39.19 furina seahorse
	furinaSeahorse();
	
	//39.22 cyno n1
	cynoN1(0);
	
	//39.53 cyno n2
	cynoN2(0);
	
	//39.78 baizhu burst heal
	baizhuBurstHeal();
	
	//40.00 update buff
	nahidaWanderingEvenstarBuff = 24.0/100 * nahida.EM;
	alliesWanderingEvenstarBuff = 8.0/100 * nahida.EM;
	
	//40.13 cyno n3
	puts("Cyno");
	addReaction(40.13, 3, 1.0);
	cynoN3(cynoAgg);
	
	//40.62 cyno n4a
	cynoN4a(0);
	
	//40.72 baizhu burst damage
	puts("Baizhu");
	addReaction(40.72, 4, 1.0);
	baizhuBurstDamage();
	
	//40.78 cyno n4b
	cynoN4b(0);
	
	//40.78 nahida tkp
	puts("Nahida");
	addReaction(40.78, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//40.79 furina seahorse
	puts("Furina");
	addReaction(40.79, 5, 1.0);
	furinaSeahorse();
	
	//41.27 furina octopus
	puts("Furina");
	addReaction(41.27, 5, 1.0);
	furinaOctopus();
	
	//41.35 cyno skill
	puts("Cyno");
	addReaction(41.35, 3, 1.0);
	cynoSkill(cynoAgg, 0);
	cynoHyperbloom();
	cynoHyperbloom();
	
	//41.67 cyno n1
	puts("Cyno");
	addReaction(41.67, 3, 1.0);
	cynoN1(cynoAgg);
	
	//41.98 cyno n2
	cynoN2(0);
	
	//42.28 baizhu burst heal
	baizhuBurstHeal();
	
	//42.38 furina seahorse
	furinaSeahorse();
	
	//42.58 cyno n3
	cynoN3(0);
	
	//43.07 cyno n4a
	puts("Cyno");
	addReaction(43.07, 3, 1.0);
	cynoN4a(cynoAgg);
	
	//43.13 nahida tkp
	puts("Nahida");
	addReaction(43.13, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//43.15 baizhu burst damage
	puts("Baizhu");
	addReaction(43.15, 4, 1.0);
	baizhuBurstDamage();
	
	//43.23 cyno n4b
	cynoN4b(0);
	
	//43.25 baizhu ohc pop
	baizhuOHC();
	
	//43.75 baizhu ohc reset
	baizhuOHCHeal = 0;
	
	//43.80 cyno skill
	puts("Cyno");
	addReaction(43.80, 3, 1.0);
	cynoSkill(cynoAgg, 0);
	
	//43.82 furina crab
	puts("Furina");
	addReaction(43.82, 5, 1.0);
	furinaCrab();
	
	//44.02 furina seahorse
	puts("Furina");
	addReaction(44.02, 5, 1.0);
	furinaSeahorse();
	
	//44.13 cyno n1
	cynoN1(0);
	cynoHyperbloom();
	cynoHyperbloom();
	
	//44.43 furina octopus
	furinaOctopus();
	
	//44.45 cyno n2	
	puts("Cyno");
	addReaction(44.45, 3, 1.0);
	cynoN2(0);
	
	//44.78 baizhu burst heal
	baizhuBurstHeal();
	
	//45.05 cyno n3
	cynoN3(0);
	
	//45.53 cyno n4a
	cynoN4a(0);
	
	//45.63 furina seahorse
	furinaSeahorse();
	
	//45.68 baizhu burst damage
	puts("Baizhu");
	addReaction(45.68, 4, 1.0);
	baizhuBurstDamage();
	
	//45.70 cyno n4b
	puts("Cyno");
	addReaction(45.70, 3, 1.0);
	cynoN4b(cynoAgg);
	
	//45.75 nahida tkp
	puts("Nahida");
	addReaction(45.75, 4, 1.5);
	nahidaTriKarmaPurification(nahidaSpread);
	
	//46.07 furina burst expires
	furinaBurstActive = false;
	
	cyno.DMGBonus -= (fanfare) * 0.23/100;
	nahida.DMGBonus -= (fanfare) * 0.23/100;
	baizhu.DMGBonus -= (fanfare) * 0.23/100;
	furina.DMGBonus -= (fanfare) * 0.23/100;
	
	cyno.incomingHB -= (fanfare) * 0.09/100;
	nahida.incomingHB -= (fanfare) * 0.09/100;
	baizhu.incomingHB -= (fanfare) * 0.09/100;
	furina.incomingHB -= (fanfare) * 0.09/100;
	
	fanfare = 0;
	prevFanfare = 0;
	
	//46.25 cyno skill
	puts("Cyno");
	addReaction(46.25, 3, 1.0);
	cynoSkill(cynoAgg, 0);
	
	
	
	
	
	
	
	
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", cyno.name, cyno.currHP * 100, cyno.currHP * cyno.HP, cyno.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", nahida.name, nahida.currHP * 100, nahida.currHP * nahida.HP, nahida.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", baizhu.name, baizhu.currHP * 100, baizhu.currHP * baizhu.HP, baizhu.HP);
	printf("%s Current HP: %.1f%% : %.0f / %.0f\n", furina.name, furina.currHP * 100, furina.currHP * furina.HP, furina.HP);
	
	puts("");
	
	printf("Fanfare: %.1f\n", fanfare);
	printf("Total Damage: %.1f\n", totalDamage);
	printf("Cyno Damage: %.1f\n", cyno.damage);
	printf("Nahida Damage: %.1f\n", nahida.damage);
	printf("Baizhu Damage: %.1f\n", baizhu.damage);
	printf("Furina Damage: %.1f\n", furina.damage);
	
	puts("");
	printf("Damage per Second: %.1f\n", totalDamage / 23.0);
	printf("Cyno DPS: %.1f\n", cyno.damage / 23.0);
	printf("Nahida DPS: %.1f\n", nahida.damage / 23.0);
	printf("Baizhu DPS: %.1f\n", baizhu.damage / 23.0);
	printf("Furina DPS: %.1f\n", furina.damage / 23.0);
	
	puts("");
	printf("Cyno Contribution: %.1f%%\n", cyno.damage / totalDamage * 100);
	printf("Nahida Contribution: %.1f%%\n", nahida.damage / totalDamage * 100);
	printf("Baizhu Contribution: %.1f%%\n", baizhu.damage / totalDamage * 100);
	printf("Furina Contribution: %.1f%%\n", furina.damage / totalDamage * 100);
	
	puts("");
	printf("Aggravates: %d, Spreads: %d, Blooms: %d\n", aggravates, spreads, blooms);
	aggravates = 0;
	spreads = 0;
	blooms = 0;
	
    return 0;
}
