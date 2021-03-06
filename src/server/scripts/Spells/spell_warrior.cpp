/*
* Copyright (C) 2016-2017 StormCore
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
* Scripts for spells with SPELLFAMILY_WARRIOR and SPELLFAMILY_GENERIC spells used by warrior players.
* Ordered alphabetically using scriptname.
* Scriptnames of files in this file should be prefixed with "spell_warr_".
*/

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellHistory.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum WarriorSpells
{
	SPELL_WARRIOR_BATTLE_CRY                    = 1719,
	SPELL_WARRIOR_BLADESTORM_PERIODIC_WHIRLWIND = 50622,
	SPELL_WARRIOR_BLOODTHIRST_HEAL = 117313,
	SPELL_WARRIOR_BLOODTHIRST_DAMAGE = 23881,
	SPELL_WARRIOR_CHARGE = 34846,
	SPELL_WARRIOR_COLOSSUS_SMASH = 86346,
	SPELL_WARRIOR_DEEP_WOUNDS_RANK_1 = 12162,
	SPELL_WARRIOR_DEEP_WOUNDS_RANK_2 = 12850,
	SPELL_WARRIOR_DEEP_WOUNDS_RANK_3 = 12868,
	SPELL_WARRIOR_DEEP_WOUNDS_PERIODIC = 12721,
	SPELL_WARRIOR_EXECUTE = 20647,
	SPELL_WARRIOR_GLYPH_OF_EXECUTION = 58367,
	SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_BUFF = 65156,
	SPELL_WARRIOR_JUGGERNAUT_CRIT_BONUS_TALENT = 64976,
	SPELL_WARRIOR_LAST_STAND_TRIGGERED = 12976,
	SPELL_WARRIOR_MORTAL_STRIKE = 12294,
	SPELL_WARRIOR_RALLYING_CRY = 97463,
	SPELL_WARRIOR_REND = 94009,
	SPELL_WARRIOR_RETALIATION_DAMAGE = 22858,
	SPELL_WARRIOR_SECOND_WIND_PROC = 29838,
	SPELL_WARRIOR_SECOND_WIND_TIME_MARKER = 202149,
	SPELL_WARRIOR_SHIELD_SLAM = 23922,
	SPELL_WARRIOR_SLAM = 50782,
	SPELL_WARRIOR_SUNDER_ARMOR = 58567,
	SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_1 = 12723,
	SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_2 = 26654,
	SPELL_WARRIOR_TAUNT = 355,
	SPELL_WARRIOR_TRAUMA_TRIGGERED = 215537,
	SPELL_WARRIOR_TRAUMA = 215538,
	SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1 = 46859,
	SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2 = 46860,
	SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1 = 64849,
	SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2 = 64850,
	SPELL_WARRIOR_VIGILANCE_PROC = 50725,
	SPELL_WARRIOR_VENGEANCE = 76691,
	SPELL_WARRIOR_VICTORIOUS       = 32216,
	SPELL_WARRIOR_VICTORY_RUSH_HEAL                 = 118779,
	SPELL_WARRIOR_HEROIC_LEAP_JUMP = 178368,
	SPELL_WARRIOR_SHOCKWAVE                         = 46968,
    SPELL_WARRIOR_SHOCKWAVE_STUN                    = 132168,
	SPELL_WARRIOR_GLYPH_OF_HEROIC_LEAP = 159708,
	SPELL_WARRIOR_GLYPH_OF_HEROIC_LEAP_BUFF = 133278,
	SPELL_WARRIOR_IMPROVED_HEROIC_LEAP = 157449,
	SPELL_WARRIOR_CHARGE_STUN = 213427,
	SPELL_WARRIOR_CHARGE_RUN = 218104,
	SPELL_WARRIOR_WARBRINGER = 103828,
	SPELL_WARRIOR_WARBRINGER_ROOT = 105771,
};

enum WarriorSpellIcons
{
	WARRIOR_ICON_ID_SUDDEN_DEATH = 1989
};


enum MiscSpells
{
	SPELL_PALADIN_BLESSING_OF_SANCTUARY = 20911,
	SPELL_PALADIN_GREATER_BLESSING_OF_SANCTUARY = 25899,
	SPELL_PRIEST_RENEWED_HOPE = 63944
};


// 152278 - Anger Management
// Updated 7.1.5
class spell_warr_anger_management : public SpellScriptLoader
{
public:
    spell_warr_anger_management() : SpellScriptLoader("spell_warr_anger_management") { }
	
	class spell_warr_anger_management_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_anger_management_AuraScript);
		
		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_BATTLE_CRY))
				return false;
			return true;
		}
		
		void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
		{
			PreventDefaultAction();
			if (Unit* caster = GetCaster())
			{
				SpellInfo const* spellInfo = eventInfo.GetDamageInfo()->GetSpellInfo();
				std::vector<SpellInfo::CostData> costs = spellInfo->CalcPowerCost(GetTarget(), SpellSchoolMask(eventInfo.GetDamageInfo()->GetSchoolMask()));
				auto m = std::find_if(costs.begin(), costs.end(), [](SpellInfo::CostData const& cost) { return cost.Power == POWER_RAGE; });
				if (m != costs.end())
					if (m->Amount >= 10)
						caster->GetSpellHistory()->ModifyCooldown(SPELL_WARRIOR_BATTLE_CRY, -1 * IN_MILLISECONDS);
					}
				}
				
				void Register() override
				{
					OnEffectProc += AuraEffectProcFn(spell_warr_anger_management_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
				}
			};
			
			AuraScript* GetAuraScript() const override
			{
				return new spell_warr_anger_management_AuraScript();
				}
			};

// 23881 - Bloodthirst 
// Updated 7.1.5
class spell_warr_bloodthirst : public SpellScriptLoader
{
public:
	spell_warr_bloodthirst() : SpellScriptLoader("spell_warr_bloodthirst") { }

	class spell_warr_bloodthirst_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_bloodthirst_SpellScript);

		void HandleDummy(SpellEffIndex /*effIndex*/)
		{
			GetCaster()->CastSpell(GetCaster(), SPELL_WARRIOR_BLOODTHIRST_HEAL, true);
		}

		void Register() override
		{
			OnEffectHit += SpellEffectFn(spell_warr_bloodthirst_SpellScript::HandleDummy, EFFECT_3, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_bloodthirst_SpellScript();
	}
};

/// Updated 4.3.4
class spell_warr_charge : public SpellScriptLoader
{
public:
	spell_warr_charge() : SpellScriptLoader("spell_warr_charge") {}

	class spell_warr_charge_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_charge_SpellScript);

		void HandleChargeHit(SpellEffIndex /*effIndex*/)
		{
			Unit* caster = GetCaster();
			Unit* target = GetExplTargetUnit();

			if (!target)
				return;

			caster->CastSpell(target, SPELL_WARRIOR_CHARGE_RUN, true);
			if (!caster->HasAura(SPELL_WARRIOR_WARBRINGER))
				caster->CastSpell(target, SPELL_WARRIOR_WARBRINGER_ROOT, true);
			else
				caster->CastSpell(target, SPELL_WARRIOR_CHARGE_STUN, true);
		}

		void Register() override
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_charge_SpellScript::HandleChargeHit, EFFECT_0, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_charge_SpellScript();
	}
};

/// Updated 4.3.4
class spell_warr_concussion_blow : public SpellScriptLoader
{
public:
	spell_warr_concussion_blow() : SpellScriptLoader("spell_warr_concussion_blow") { }

	class spell_warr_concussion_blow_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_concussion_blow_SpellScript);

		void HandleDummy(SpellEffIndex /*effIndex*/)
		{
			SetHitDamage(CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetEffectValue()));
		}

		void Register() override
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_concussion_blow_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_concussion_blow_SpellScript();
	}
};

/// Updated 4.3.4
class spell_warr_execute : public SpellScriptLoader
{
public:
	spell_warr_execute() : SpellScriptLoader("spell_warr_execute") { }

	class spell_warr_execute_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_execute_SpellScript);

		void HandleEffect(SpellEffIndex /*effIndex*/)
		{
			/*Unit* caster = GetCaster();
			if (GetHitUnit())
			{
			SpellInfo const* spellInfo = GetSpellInfo();
			int32 rageUsed = std::min<int32>(200 - spellInfo->CalcPowerCost(caster, SpellSchoolMask(spellInfo->SchoolMask)), caster->GetPower(POWER_RAGE));
			int32 newRage = std::max<int32>(0, caster->GetPower(POWER_RAGE) - rageUsed);

			// Sudden Death rage save
			if (AuraEffect* aurEff = caster->GetAuraEffect(SPELL_AURA_PROC_TRIGGER_SPELL, SPELLFAMILY_GENERIC, WARRIOR_ICON_ID_SUDDEN_DEATH, EFFECT_0))
			{
			int32 ragesave = aurEff->GetSpellInfo()->Effects[EFFECT_0].CalcValue() * 10;
			newRage = std::max(newRage, ragesave);
			}

			caster->SetPower(POWER_RAGE, uint32(newRage));

			/// Formula taken from the DBC: "${10+$AP*0.437*$m1/100}"
			int32 baseDamage = int32(10 + caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.437f * GetEffectValue() / 100.0f);
			/// Formula taken from the DBC: "${$ap*0.874*$m1/100-1} = 20 rage"
			int32 moreDamage = int32(rageUsed * (caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.874f * GetEffectValue() / 100.0f - 1) / 200);
			SetHitDamage(baseDamage + moreDamage);
			}*/
		}

		void Register() override
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_execute_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_execute_SpellScript();
	}
};

// 59725 - Improved Spell Reflection
class spell_warr_improved_spell_reflection : public SpellScriptLoader
{
public:
	spell_warr_improved_spell_reflection() : SpellScriptLoader("spell_warr_improved_spell_reflection") { }

	class spell_warr_improved_spell_reflection_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_improved_spell_reflection_SpellScript);

		void FilterTargets(std::list<WorldObject*>& unitList)
		{
			if (GetCaster())
				unitList.remove(GetCaster());
		}

		void Register() override
		{
			OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_improved_spell_reflection_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_PARTY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_improved_spell_reflection_SpellScript();
	}
};

// 5246 - Intimidating Shout
class spell_warr_intimidating_shout : public SpellScriptLoader
{
public:
	spell_warr_intimidating_shout() : SpellScriptLoader("spell_warr_intimidating_shout") { }

	class spell_warr_intimidating_shout_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_intimidating_shout_SpellScript);

		void FilterTargets(std::list<WorldObject*>& unitList)
		{
			unitList.remove(GetExplTargetWorldObject());
		}

		void Register() override
		{
			OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
			OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_intimidating_shout_SpellScript();
	}
};

// -84583 Lambs to the Slaughter
class spell_warr_lambs_to_the_slaughter : public SpellScriptLoader
{
public:
	spell_warr_lambs_to_the_slaughter() : SpellScriptLoader("spell_warr_lambs_to_the_slaughter") { }

	class spell_warr_lambs_to_the_slaughter_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_lambs_to_the_slaughter_AuraScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_MORTAL_STRIKE) ||
				!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_REND))
				return false;
			return true;
		}

		void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
		{
			if (Aura* aur = eventInfo.GetProcTarget()->GetAura(SPELL_WARRIOR_REND, GetTarget()->GetGUID()))
				aur->SetDuration(aur->GetSpellInfo()->GetMaxDuration(), true);

		}

		void Register() override
		{
			OnEffectProc += AuraEffectProcFn(spell_warr_lambs_to_the_slaughter_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
		}
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_lambs_to_the_slaughter_AuraScript();
	}
};

/// Updated 4.3.4
// 12975 - Last Stand
class spell_warr_last_stand : public SpellScriptLoader
{
public:
	spell_warr_last_stand() : SpellScriptLoader("spell_warr_last_stand") { }

	class spell_warr_last_stand_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_last_stand_SpellScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_LAST_STAND_TRIGGERED))
				return false;
			return true;
		}

		void HandleDummy(SpellEffIndex /*effIndex*/)
		{
			Unit* caster = GetCaster();
			int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(GetEffectValue()));
			caster->CastCustomSpell(caster, SPELL_WARRIOR_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
		}

		void Register() override
		{
			// add dummy effect spell handler to Last Stand
			OnEffectHit += SpellEffectFn(spell_warr_last_stand_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_last_stand_SpellScript();
	}
};

// 7384 - Overpower
class spell_warr_overpower : public SpellScriptLoader
{
public:
	spell_warr_overpower() : SpellScriptLoader("spell_warr_overpower") { }

	class spell_warr_overpower_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_overpower_SpellScript);

		void HandleEffect(SpellEffIndex /*effIndex*/)
		{
			uint32 spellId = 0;
			if (GetCaster()->HasAura(SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_1))
				spellId = SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_1;
			else if (GetCaster()->HasAura(SPELL_WARRIOR_UNRELENTING_ASSAULT_RANK_2))
				spellId = SPELL_WARRIOR_UNRELENTING_ASSAULT_TRIGGER_2;

			if (!spellId)
				return;

			if (Player* target = GetHitPlayer())
				if (target->IsNonMeleeSpellCast(false, false, true)) // UNIT_STATE_CASTING should not be used here, it's present during a tick for instant casts
					target->CastSpell(target, spellId, true);
		}

		void Register() override
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_overpower_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_ANY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_overpower_SpellScript();
	}
};

// 97462 - Rallying Cry
class spell_warr_rallying_cry : public SpellScriptLoader
{
public:
	spell_warr_rallying_cry() : SpellScriptLoader("spell_warr_rallying_cry") { }

	class spell_warr_rallying_cry_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_rallying_cry_SpellScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_RALLYING_CRY))
				return false;
			return true;
		}

		bool Load() override
		{
			return GetCaster()->GetTypeId() == TYPEID_PLAYER;
		}

		void HandleScript(SpellEffIndex /*effIndex*/)
		{
			int32 basePoints0 = int32(GetHitUnit()->CountPctFromMaxHealth(GetEffectValue()));

			GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_WARRIOR_RALLYING_CRY, &basePoints0, NULL, NULL, true);
		}

		void Register() override
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_rallying_cry_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_rallying_cry_SpellScript();
	}
};


class spell_warr_ravager : public SpellScriptLoader
{
public:
    spell_warr_ravager() : SpellScriptLoader("spell_warr_ravager") { }
	
	class spell_warr_ravager_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_ravager_SpellScript);
		
		void HandleDummy(SpellEffIndex /*effIndex*/)
		{
			if (WorldLocation const* dest = GetExplTargetDest())
				GetCaster()->CastSpell(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), 227876, true);
		}
		
		void Register() override
		{
			OnEffectHit += SpellEffectFn(spell_warr_ravager_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
		}
	};
	
	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_ravager_SpellScript();
	}
 };
	
// 94009 - Rend
class spell_warr_rend : public SpellScriptLoader
{
public:
	spell_warr_rend() : SpellScriptLoader("spell_warr_rend") { }

	class spell_warr_rend_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_rend_AuraScript);

		void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
		{
			if (Unit* caster = GetCaster())
			{
				canBeRecalculated = false;

				// $0.25 * (($MWB + $mwb) / 2 + $AP / 14 * $MWS) bonus per tick
				float ap = caster->GetTotalAttackPowerValue(BASE_ATTACK);
				int32 mws = caster->GetBaseAttackTime(BASE_ATTACK);
				float mwbMin = caster->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE);
				float mwbMax = caster->GetWeaponDamageRange(BASE_ATTACK, MAXDAMAGE);
				float mwb = ((mwbMin + mwbMax) / 2 + ap * mws / 14000) * 0.25f;
				amount += int32(caster->ApplyEffectModifiers(GetSpellInfo(), aurEff->GetEffIndex(), mwb));
			}
		}

		void Register() override
		{
			DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_rend_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
		}
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_rend_AuraScript();
	}
};

// 20230 - Retaliation
class spell_warr_retaliation : public SpellScriptLoader
{
public:
	spell_warr_retaliation() : SpellScriptLoader("spell_warr_retaliation") { }

	class spell_warr_retaliation_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_retaliation_AuraScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_RETALIATION_DAMAGE))
				return false;
			return true;
		}

		bool CheckProc(ProcEventInfo& eventInfo)
		{
			// check attack comes not from behind and warrior is not stunned
			return GetTarget()->isInFront(eventInfo.GetActor(), float(M_PI)) && !GetTarget()->HasUnitState(UNIT_STATE_STUNNED);
		}

		void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
		{
			PreventDefaultAction();
			GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_WARRIOR_RETALIATION_DAMAGE, true, NULL, aurEff);
		}

		void Register() override
		{
			DoCheckProc += AuraCheckProcFn(spell_warr_retaliation_AuraScript::CheckProc);
			OnEffectProc += AuraEffectProcFn(spell_warr_retaliation_AuraScript::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
		}
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_retaliation_AuraScript();
	}
};

// 64380, 65941 - Shattering Throw
class spell_warr_shattering_throw : public SpellScriptLoader
{
public:
	spell_warr_shattering_throw() : SpellScriptLoader("spell_warr_shattering_throw") { }

	class spell_warr_shattering_throw_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_shattering_throw_SpellScript);

		void HandleScript(SpellEffIndex effIndex)
		{
			PreventHitDefaultEffect(effIndex);

			// remove shields, will still display immune to damage part
			if (Unit* target = GetHitUnit())
				target->RemoveAurasWithMechanic(1 << MECHANIC_IMMUNE_SHIELD, AURA_REMOVE_BY_ENEMY_SPELL);
		}

		void Register() override
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_shattering_throw_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_shattering_throw_SpellScript();
	}
};

/// Updated 4.3.4
class spell_warr_slam : public SpellScriptLoader
{
public:
	spell_warr_slam() : SpellScriptLoader("spell_warr_slam") { }

	class spell_warr_slam_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_slam_SpellScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SLAM))
				return false;
			return true;
		}

		void HandleDummy(SpellEffIndex /*effIndex*/)
		{
			if (GetHitUnit())
				GetCaster()->CastCustomSpell(SPELL_WARRIOR_SLAM, SPELLVALUE_BASE_POINT0, GetEffectValue(), GetHitUnit(), TRIGGERED_FULL_MASK);
		}

		void Register() override
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_slam_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_slam_SpellScript();
	}
};

// 29838 - Second Wind Proc
/// 7.x
class spell_warr_second_wind_proc : public SpellScriptLoader
{
public:
	spell_warr_second_wind_proc() : SpellScriptLoader("spell_warr_second_wind_proc") { }

	class spell_warr_second_wind_proc_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_second_wind_proc_AuraScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SECOND_WIND_PROC) || !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SECOND_WIND_TIME_MARKER))
				return false;
			return true;
		}

		void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
		{
			PreventDefaultAction();
			GetCaster()->AddAura(SPELL_WARRIOR_SECOND_WIND_TIME_MARKER, GetCaster());
		}

		void Register() override
		{
			OnEffectProc += AuraEffectProcFn(spell_warr_second_wind_proc_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
		}

	private:
		uint32 seconds;
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_second_wind_proc_AuraScript();
	}
};

class spell_warr_second_wind_trigger : public SpellScriptLoader
{
public:
	spell_warr_second_wind_trigger() : SpellScriptLoader("spell_warr_second_wind_trigger") { }

	class spell_warr_second_wind_trigger_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_second_wind_trigger_AuraScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SECOND_WIND_TIME_MARKER))
				return false;
			return true;
		}

		void OnPeriodic(AuraEffect const* /*aurEff*/)
		{
			if (GetCaster()->HasAura(SPELL_WARRIOR_SECOND_WIND_TIME_MARKER))
				PreventDefaultAction();
		}

		void Register() override
		{
			OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_second_wind_trigger_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_OBS_MOD_HEALTH);
		}
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_second_wind_trigger_AuraScript();
	}
};

// 52437 - Sudden Death
class spell_warr_sudden_death : public SpellScriptLoader
{
public:
	spell_warr_sudden_death() : SpellScriptLoader("spell_warr_sudden_death") { }

	class spell_warr_sudden_death_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_sudden_death_AuraScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_COLOSSUS_SMASH))
				return false;
			return true;
		}

		void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
		{
			// Remove cooldown on Colossus Smash
			if (Player* player = GetTarget()->ToPlayer())
				player->GetSpellHistory()->ResetCooldown(SPELL_WARRIOR_COLOSSUS_SMASH, true);
		}

		void Register() override
		{
			AfterEffectApply += AuraEffectRemoveFn(spell_warr_sudden_death_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL); // correct?
		}
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_sudden_death_AuraScript();
	}
};

// 12328, 18765, 35429 - Sweeping Strikes
class spell_warr_sweeping_strikes : public SpellScriptLoader
{
public:
	spell_warr_sweeping_strikes() : SpellScriptLoader("spell_warr_sweeping_strikes") { }

	class spell_warr_sweeping_strikes_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_sweeping_strikes_AuraScript);

	public:
		spell_warr_sweeping_strikes_AuraScript()
		{
			_procTarget = nullptr;
		}

	private:
		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_1) || !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_2))
				return false;
			return true;
		}

		bool CheckProc(ProcEventInfo& eventInfo)
		{
			_procTarget = eventInfo.GetActor()->SelectNearbyTarget(eventInfo.GetProcTarget());
			return _procTarget != nullptr;
		}

		void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
		{
			PreventDefaultAction();
			if (eventInfo.GetDamageInfo())
			{
				SpellInfo const* spellInfo = eventInfo.GetDamageInfo()->GetSpellInfo();
				if (spellInfo && (spellInfo->Id == SPELL_WARRIOR_BLADESTORM_PERIODIC_WHIRLWIND || (spellInfo->Id == SPELL_WARRIOR_EXECUTE && !_procTarget->HasAuraState(AURA_STATE_HEALTHLESS_20_PERCENT))))
				{
					// If triggered by Execute (while target is not under 20% hp) or Bladestorm deals normalized weapon damage
					GetTarget()->CastSpell(_procTarget, SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_2, true, NULL, aurEff);
				}
				else
				{
					int32 damage = eventInfo.GetDamageInfo()->GetDamage();
					GetTarget()->CastCustomSpell(SPELL_WARRIOR_SWEEPING_STRIKES_EXTRA_ATTACK_1, SPELLVALUE_BASE_POINT0, damage, _procTarget, true, NULL, aurEff);
				}
			}
		}

		void Register() override
		{
			DoCheckProc += AuraCheckProcFn(spell_warr_sweeping_strikes_AuraScript::CheckProc);
			OnEffectProc += AuraEffectProcFn(spell_warr_sweeping_strikes_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
		}

	private:
		Unit* _procTarget;
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_sweeping_strikes_AuraScript();
	}
};

// -46951 - Sword and Board
class spell_warr_sword_and_board : public SpellScriptLoader
{
public:
	spell_warr_sword_and_board() : SpellScriptLoader("spell_warr_sword_and_board") { }

	class spell_warr_sword_and_board_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_sword_and_board_AuraScript);

	private:
		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHIELD_SLAM))
				return false;
			return true;
		}

		void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
		{
			// Remove cooldown on Shield Slam
			if (Player* player = GetTarget()->ToPlayer())
				player->GetSpellHistory()->ResetCooldown(SPELL_WARRIOR_SHIELD_SLAM, true);
		}

		void Register() override
		{
			OnEffectProc += AuraEffectProcFn(spell_warr_sword_and_board_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
		}
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_sword_and_board_AuraScript();
	}
};

// 215538 - Trauma
/// 7.x
class spell_warr_trauma : public SpellScriptLoader
{
public:
	spell_warr_trauma() : SpellScriptLoader("spell_warr_trauma") { }

	class spell_warr_trauma_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_trauma_AuraScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_TRAUMA) || !sSpellMgr->GetSpellInfo(SPELL_WARRIOR_TRAUMA_TRIGGERED))
				return false;
			return true;
		}

		void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
		{
			PreventDefaultAction();
			// Proc only with Slam & Whirlwind
			if (!(eventInfo.GetDamageInfo()->GetSpellInfo()->SpellFamilyFlags[0] & 0x200000 || eventInfo.GetDamageInfo()->GetSpellInfo()->SpellFamilyFlags[1] & 0x4))
				return;

			int32 damage = eventInfo.GetDamageInfo()->GetDamage() * 0.25 / 6;
			GetCaster()->CastCustomSpell(eventInfo.GetActionTarget(), SPELL_WARRIOR_TRAUMA_TRIGGERED, &damage, NULL, NULL, true);
		}

		void Register() override
		{
			OnEffectProc += AuraEffectProcFn(spell_warr_trauma_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
		}
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_trauma_AuraScript();
	}
};

// 34428 - Victory Rush
class spell_warr_victory_rush : public SpellScriptLoader

{
public:
    spell_warr_victory_rush() : SpellScriptLoader("spell_warr_victory_rush") { }
	
	class spell_warr_victory_rush_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_victory_rush_SpellScript);
		
		bool Validate(SpellInfo const* /*spellInfo*/) override	
		{
			return ValidateSpellInfo
			({
				SPELL_WARRIOR_VICTORIOUS,
				SPELL_WARRIOR_VICTORY_RUSH_HEAL
			});
		}
		void HandleHeal()
		{
			
				
			
			Unit* caster = GetCaster();
			caster->CastSpell(caster, SPELL_WARRIOR_VICTORY_RUSH_HEAL, true);
			caster->RemoveAurasDueToSpell(SPELL_WARRIOR_VICTORIOUS);
		}

		void Register() override
		{
			AfterCast += SpellCastFn(spell_warr_victory_rush_SpellScript::HandleHeal);
			
		}
	};
	
	SpellScript* GetSpellScript() const override

	{
		return new spell_warr_victory_rush_SpellScript();		
	}
};

// 50720 - Vigilance
class spell_warr_vigilance : public SpellScriptLoader
{
public:
	spell_warr_vigilance() : SpellScriptLoader("spell_warr_vigilance") { }

	class spell_warr_vigilance_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_warr_vigilance_AuraScript);

	public:
		spell_warr_vigilance_AuraScript()
		{
			_procTarget = nullptr;
		}

	private:
		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_VENGEANCE))
				return false;
			return true;
		}

		bool Load() override
		{
			_procTarget = NULL;
			return true;
		}

		/*
		bool CheckProc(ProcEventInfo& eventInfo)
		{
		_procTarget = GetCaster();
		return _procTarget && eventInfo.GetDamageInfo();
		}

		void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
		{
		PreventDefaultAction();
		int32 damage = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetSpellInfo()->Effects[EFFECT_1].CalcValue()));

		GetTarget()->CastSpell(_procTarget, SPELL_WARRIOR_VIGILANCE_PROC, true, NULL, aurEff);
		_procTarget->CastCustomSpell(_procTarget, SPELL_WARRIOR_VENGEANCE, &damage, &damage, &damage, true, NULL, aurEff);
		}
		*/

		void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
		{
			if (Unit* caster = GetCaster())
			{
				if (caster->HasAura(SPELL_WARRIOR_VENGEANCE))
					caster->RemoveAurasDueToSpell(SPELL_WARRIOR_VENGEANCE);
			}
		}

		void Register() override
		{
			//DoCheckProc += AuraCheckProcFn(spell_warr_vigilance_AuraScript::CheckProc);
			//OnEffectProc += AuraEffectProcFn(spell_warr_vigilance_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
			OnEffectRemove += AuraEffectRemoveFn(spell_warr_vigilance_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
		}

	private:
		Unit* _procTarget;
	};

	AuraScript* GetAuraScript() const override
	{
		return new spell_warr_vigilance_AuraScript();
	}
};

// 50725 Vigilance
class spell_warr_vigilance_trigger : public SpellScriptLoader
{
public:
	spell_warr_vigilance_trigger() : SpellScriptLoader("spell_warr_vigilance_trigger") { }

	class spell_warr_vigilance_trigger_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_vigilance_trigger_SpellScript);

		void HandleScript(SpellEffIndex effIndex)
		{
			PreventHitDefaultEffect(effIndex);

			// Remove Taunt cooldown
			if (Player* target = GetHitPlayer())
				target->GetSpellHistory()->ResetCooldown(SPELL_WARRIOR_TAUNT, true);
		}

		void Register() override
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_vigilance_trigger_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_vigilance_trigger_SpellScript();
	}
};

// Heroic leap - 6544
class spell_warr_heroic_leap : public SpellScriptLoader
{
public:
	spell_warr_heroic_leap() : SpellScriptLoader("spell_warr_heroic_leap") { }

	class spell_warr_heroic_leap_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_heroic_leap_SpellScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_HEROIC_LEAP_JUMP))
				return false;
			return true;
		}

		SpellCastResult CheckElevation()
		{
			if (WorldLocation const* dest = GetExplTargetDest())
			{
				if (GetCaster()->HasUnitMovementFlag(MOVEMENTFLAG_ROOT))
					return SPELL_FAILED_ROOTED;

				if (GetCaster()->GetMap()->Instanceable())
				{
					float range = GetSpellInfo()->GetMaxRange(true, GetCaster()) * 1.5f;

					PathGenerator generatedPath(GetCaster());
					generatedPath.SetPathLengthLimit(range);

					bool result = generatedPath.CalculatePath(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), false, true);
					if (generatedPath.GetPathType() & PATHFIND_SHORT)
						return SPELL_FAILED_OUT_OF_RANGE;
					else if (!result || generatedPath.GetPathType() & PATHFIND_NOPATH)
					{
						result = generatedPath.CalculatePath(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), false, false);
						if (generatedPath.GetPathType() & PATHFIND_SHORT)
							return SPELL_FAILED_OUT_OF_RANGE;
						else if (!result || generatedPath.GetPathType() & PATHFIND_NOPATH)
							return SPELL_FAILED_NOPATH;
					}
				}
				else if (dest->GetPositionZ() > GetCaster()->GetPositionZ() + 4.0f)
					return SPELL_FAILED_NOPATH;

				return SPELL_CAST_OK;
			}

			return SPELL_FAILED_NO_VALID_TARGETS;
		}

		void HandleDummy(SpellEffIndex /*effIndex*/)
		{
			if (WorldLocation* dest = GetHitDest())
				GetCaster()->CastSpell(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), SPELL_WARRIOR_HEROIC_LEAP_JUMP, true);
		}

		void Register() override
		{
			OnCheckCast += SpellCheckCastFn(spell_warr_heroic_leap_SpellScript::CheckElevation);
			OnEffectHit += SpellEffectFn(spell_warr_heroic_leap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_heroic_leap_SpellScript();
	}
};

// Heroic Leap (triggered by Heroic Leap (6544)) - 178368
class spell_warr_heroic_leap_jump : public SpellScriptLoader
{
public:
	spell_warr_heroic_leap_jump() : SpellScriptLoader("spell_warr_heroic_leap_jump") { }

	class spell_warr_heroic_leap_jump_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_heroic_leap_jump_SpellScript);

		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_GLYPH_OF_HEROIC_LEAP) ||
				!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_GLYPH_OF_HEROIC_LEAP_BUFF) ||
				!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_IMPROVED_HEROIC_LEAP) ||
				!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_TAUNT))
				return false;
			return true;
		}

		void AfterJump(SpellEffIndex /*effIndex*/)
		{
			if (GetCaster()->HasAura(SPELL_WARRIOR_GLYPH_OF_HEROIC_LEAP))
				GetCaster()->CastSpell(GetCaster(), SPELL_WARRIOR_GLYPH_OF_HEROIC_LEAP_BUFF, true);
			if (GetCaster()->HasAura(SPELL_WARRIOR_IMPROVED_HEROIC_LEAP))
				GetCaster()->GetSpellHistory()->ResetCooldown(SPELL_WARRIOR_TAUNT, true);
		}

		void Register() override
		{
			OnEffectHit += SpellEffectFn(spell_warr_heroic_leap_jump_SpellScript::AfterJump, EFFECT_1, SPELL_EFFECT_JUMP_DEST);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_warr_heroic_leap_jump_SpellScript();
	}
};

// Shockwave - 46968
class spell_warr_shockwave : public SpellScriptLoader
{
public:
	spell_warr_shockwave() : SpellScriptLoader("spell_warr_shockwave") { }

	class spell_warr_shockwave_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_warr_shockwave_SpellScript);

		int32 hitTargets = 0;
		
		bool Validate(SpellInfo const* /*spellInfo*/) override
		{
			if (!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHOCKWAVE) ||
				!sSpellMgr->GetSpellInfo(SPELL_WARRIOR_SHOCKWAVE_STUN))
				return false;
			return true;
		}
		
		void HandleDamage(SpellEffIndex /*effIndex*/)
		{
			if (Unit* caster = GetCaster())
				if (Unit* target = GetHitUnit()) {
					caster->CastSpell(target, SPELL_WARRIOR_SHOCKWAVE_STUN, true);
					hitTargets++;
				}
		}

		// Cooldown reduced by 20 sec if it strikes at least 3 targets.
		void HandleAfterHit()
		{
			if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
				return;
			
			if (hitTargets >= GetSpellInfo()->GetEffect(EFFECT_0)->BasePoints) {
				GetCaster()->ToPlayer()->GetSpellHistory()->ModifyCooldown(SPELL_WARRIOR_SHOCKWAVE, -(GetSpellInfo()->GetEffect(EFFECT_3)->BasePoints * IN_MILLISECONDS));
				hitTargets = 0;
			}
		}

		void Register()
		{
			OnEffectHitTarget += SpellEffectFn(spell_warr_shockwave_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
			AfterCast += SpellCastFn(spell_warr_shockwave_SpellScript::HandleAfterHit);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_warr_shockwave_SpellScript();
	}
};

void AddSC_warrior_spell_scripts()
{
	new spell_warr_anger_management();
	new spell_warr_bloodthirst();
	new spell_warr_charge();
	new spell_warr_concussion_blow();
	new spell_warr_execute();
	new spell_warr_improved_spell_reflection();
	new spell_warr_intimidating_shout();
	new spell_warr_lambs_to_the_slaughter();
	new spell_warr_last_stand();
	new spell_warr_overpower();
	new spell_warr_rallying_cry();
	new spell_warr_ravager();
	new spell_warr_rend();
	new spell_warr_retaliation();
	new spell_warr_second_wind_proc();
	new spell_warr_second_wind_trigger();
	new spell_warr_shattering_throw();
	new spell_warr_slam();
	new spell_warr_sudden_death();
	new spell_warr_sweeping_strikes();
	new spell_warr_sword_and_board();
	new spell_warr_trauma();
	new spell_warr_victory_rush();
	new spell_warr_vigilance();
	new spell_warr_vigilance_trigger();
	new spell_warr_heroic_leap();
	new spell_warr_heroic_leap_jump();
	new spell_warr_shockwave();
}