﻿using Mesen.GUI.Config;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Mesen.GUI.Forms.Config
{
	public partial class frmEmulationConfig : BaseConfigForm
	{
		public frmEmulationConfig()
		{
			InitializeComponent();
			if(DesignMode) {
				return;
			}

			Entity = ConfigManager.Config.Emulation.Clone();

			AddBinding(nameof(EmulationConfig.EmulationSpeed), nudEmulationSpeed);
			AddBinding(nameof(EmulationConfig.TurboSpeed), nudTurboSpeed);
			AddBinding(nameof(EmulationConfig.RewindSpeed), nudRewindSpeed);
			AddBinding(nameof(EmulationConfig.Region), cboRegion);
			AddBinding(nameof(EmulationConfig.RunAheadFrames), nudRunAheadFrames);

			AddBinding(nameof(EmulationConfig.RamPowerOnState), cboRamPowerOnState);
			AddBinding(nameof(EmulationConfig.EnableRandomPowerOnState), chkEnableRandomPowerOnState);
			AddBinding(nameof(EmulationConfig.EnableStrictBoardMappings), chkEnableStrictBoardMappings);

			AddBinding(nameof(EmulationConfig.PpuExtraScanlinesBeforeNmi), nudExtraScanlinesBeforeNmi);
			AddBinding(nameof(EmulationConfig.PpuExtraScanlinesAfterNmi), nudExtraScanlinesAfterNmi);
			AddBinding(nameof(EmulationConfig.GsuClockSpeed), nudGsuClockSpeed);
		}

		protected override void OnApply()
		{
			ConfigManager.Config.Emulation = (EmulationConfig)this.Entity;
			ConfigManager.ApplyChanges();
		}

		private void nudGsuClockSpeed_Leave(object sender, EventArgs e)
		{
			nudGsuClockSpeed.Value = Math.Ceiling(nudGsuClockSpeed.Value / 100) * 100;
		}
	}
}
