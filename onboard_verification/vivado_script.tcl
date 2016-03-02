
################################################################
# This is a generated script based on design: design_1
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2014.4
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   puts "ERROR: This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source design_1_script.tcl

# If you do not already have a project created,
# you can create a project using the following command:
create_project -force project_1 myproj -part xc7z045ffg900-1


# CHANGE DESIGN NAME HERE
set design_name design_1

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
create_bd_design $design_name

# CHECKING IF PROJECT EXISTS
if { [get_projects -quiet] eq "" } {
   puts "ERROR: Please open or create a project!"
   return 1
}


# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "ERROR: Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      puts "INFO: Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   puts "INFO: Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "ERROR: Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   puts "INFO: Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   puts "INFO: Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

puts "INFO: Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   puts $errMsg
   return $nRet
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     puts "ERROR: Unable to find parent cell <$parentCell>!"
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     puts "ERROR: Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set DDR [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR ]
  set FIXED_IO [ create_bd_intf_port -mode Master -vlnv xilinx.com:display_processing_system7:fixedio_rtl:1.0 FIXED_IO ]

  # Create ports

  # Create instance: axi_mem_intercon, and set properties
  set axi_mem_intercon [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 axi_mem_intercon ]
  set_property -dict [ list CONFIG.NUM_MI {1} CONFIG.NUM_SI {13}  ] $axi_mem_intercon

  #Add decoder HLS IP
  file delete -force vivado_hls_ip
  file mkdir vivado_hls_ip
  set_property ip_repo_paths vivado_hls_ip [current_fileset]
  update_ip_catalog
  update_ip_catalog -add_ip hls_proj/solution1/impl/ip/xilinx_com_hls_decode_main_1_0.zip -repo_path vivado_hls_ip

  # Create instance: decode_main_0, and set properties
  set decode_main_0 [ create_bd_cell -type ip -vlnv xilinx.com:hls:decode_main:1.0 decode_main_0 ]
  set_property -dict [ list \
  CONFIG.C_M_AXI_NALU_BUF_TARGET_ADDR {0x11000000} CONFIG.C_M_AXI_NALU_BUF_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_FRAME_NUM_TARGET_ADDR {0x14000000} CONFIG.C_M_AXI_PIC_FRAME_NUM_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_INFO_MVD_L0_TARGET_ADDR {0x1b000000} CONFIG.C_M_AXI_PIC_INFO_MVD_L0_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_INFO_MVD_L1_TARGET_ADDR {0x1c000000} CONFIG.C_M_AXI_PIC_INFO_MVD_L1_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_INFO_REFIDXL0_TARGET_ADDR {0x19000000} CONFIG.C_M_AXI_PIC_INFO_REFIDXL0_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_INFO_REFIDXL1_TARGET_ADDR {0x1a000000} CONFIG.C_M_AXI_PIC_INFO_REFIDXL1_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_MEMOCCUPIED_TARGET_ADDR {0x13000000} CONFIG.C_M_AXI_PIC_MEMOCCUPIED_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_PICORDER_NUM_TARGET_ADDR {0x12000000} CONFIG.C_M_AXI_PIC_PICORDER_NUM_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_SCHROMA_0_TARGET_ADDR {0x18000000} CONFIG.C_M_AXI_PIC_SCHROMA_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_SCHROMA_1_TARGET_ADDR {0x1d000000} CONFIG.C_M_AXI_PIC_SCHROMA_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_SLUMA_TARGET_ADDR {0x17000000} CONFIG.C_M_AXI_PIC_SLUMA_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_TYPE_TARGET_ADDR {0x15000000} CONFIG.C_M_AXI_PIC_TYPE_DATA_WIDTH {32} \
  CONFIG.C_M_AXI_PIC_USE_FOR_CHAR_TARGET_ADDR {0x16000000} CONFIG.C_M_AXI_PIC_USE_FOR_CHAR_DATA_WIDTH {32} ] $decode_main_0

  # Create instance: processing_system7_0, and set properties
  set processing_system7_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0 ]
  set_property -dict [ list CONFIG.PCW_APU_PERIPHERAL_FREQMHZ {666} CONFIG.PCW_FCLK0_PERIPHERAL_CLKSRC {IO PLL} CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {100} CONFIG.PCW_USE_S_AXI_HP0 {1}  ] $processing_system7_0

  # Create instance: processing_system7_0_axi_periph, and set properties
  set processing_system7_0_axi_periph [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 processing_system7_0_axi_periph ]
  set_property -dict [ list CONFIG.NUM_MI {1}  ] $processing_system7_0_axi_periph

  # Create instance: rst_processing_system7_0_96M, and set properties
  set rst_processing_system7_0_96M [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 rst_processing_system7_0_96M ]

  # Create interface connections
  connect_bd_intf_net -intf_net axi_mem_intercon_M00_AXI [get_bd_intf_pins axi_mem_intercon/M00_AXI] [get_bd_intf_pins processing_system7_0/S_AXI_HP0]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_NALU_BUF [get_bd_intf_pins axi_mem_intercon/S00_AXI] [get_bd_intf_pins decode_main_0/M_AXI_NALU_BUF]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_FRAME_NUM [get_bd_intf_pins axi_mem_intercon/S03_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_FRAME_NUM]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_INFO_MVD_L0 [get_bd_intf_pins axi_mem_intercon/S10_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_INFO_MVD_L0]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_INFO_MVD_L1 [get_bd_intf_pins axi_mem_intercon/S11_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_INFO_MVD_L1]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_INFO_REFIDXL0 [get_bd_intf_pins axi_mem_intercon/S08_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_INFO_REFIDXL0]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_INFO_REFIDXL1 [get_bd_intf_pins axi_mem_intercon/S09_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_INFO_REFIDXL1]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_MEMOCCUPIED [get_bd_intf_pins axi_mem_intercon/S02_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_MEMOCCUPIED]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_PICORDER_NUM [get_bd_intf_pins axi_mem_intercon/S01_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_PICORDER_NUM]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_SCHROMA_0 [get_bd_intf_pins axi_mem_intercon/S07_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_SCHROMA_0]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_SCHROMA_1 [get_bd_intf_pins axi_mem_intercon/S12_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_SCHROMA_1]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_SLUMA [get_bd_intf_pins axi_mem_intercon/S06_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_SLUMA]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_TYPE [get_bd_intf_pins axi_mem_intercon/S04_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_TYPE]
  connect_bd_intf_net -intf_net decode_main_0_M_AXI_PIC_USE_FOR_CHAR [get_bd_intf_pins axi_mem_intercon/S05_AXI] [get_bd_intf_pins decode_main_0/M_AXI_PIC_USE_FOR_CHAR]
  connect_bd_intf_net -intf_net processing_system7_0_DDR [get_bd_intf_ports DDR] [get_bd_intf_pins processing_system7_0/DDR]
  connect_bd_intf_net -intf_net processing_system7_0_FIXED_IO [get_bd_intf_ports FIXED_IO] [get_bd_intf_pins processing_system7_0/FIXED_IO]
  connect_bd_intf_net -intf_net processing_system7_0_M_AXI_GP0 [get_bd_intf_pins processing_system7_0/M_AXI_GP0] [get_bd_intf_pins processing_system7_0_axi_periph/S00_AXI]
  connect_bd_intf_net -intf_net processing_system7_0_axi_periph_M00_AXI [get_bd_intf_pins decode_main_0/S_AXI_AXI4LITES] [get_bd_intf_pins processing_system7_0_axi_periph/M00_AXI]

  # Create port connections
  connect_bd_net -net processing_system7_0_FCLK_CLK0 [get_bd_pins axi_mem_intercon/ACLK] [get_bd_pins axi_mem_intercon/M00_ACLK] [get_bd_pins axi_mem_intercon/S00_ACLK] [get_bd_pins axi_mem_intercon/S01_ACLK] [get_bd_pins axi_mem_intercon/S02_ACLK] [get_bd_pins axi_mem_intercon/S03_ACLK] [get_bd_pins axi_mem_intercon/S04_ACLK] [get_bd_pins axi_mem_intercon/S05_ACLK] [get_bd_pins axi_mem_intercon/S06_ACLK] [get_bd_pins axi_mem_intercon/S07_ACLK] [get_bd_pins axi_mem_intercon/S08_ACLK] [get_bd_pins axi_mem_intercon/S09_ACLK] [get_bd_pins axi_mem_intercon/S10_ACLK] [get_bd_pins axi_mem_intercon/S11_ACLK] [get_bd_pins axi_mem_intercon/S12_ACLK] [get_bd_pins decode_main_0/aclk] [get_bd_pins processing_system7_0/FCLK_CLK0] [get_bd_pins processing_system7_0/M_AXI_GP0_ACLK] [get_bd_pins processing_system7_0/S_AXI_HP0_ACLK] [get_bd_pins processing_system7_0_axi_periph/ACLK] [get_bd_pins processing_system7_0_axi_periph/M00_ACLK] [get_bd_pins processing_system7_0_axi_periph/S00_ACLK] [get_bd_pins rst_processing_system7_0_96M/slowest_sync_clk]
  connect_bd_net -net processing_system7_0_FCLK_RESET0_N [get_bd_pins processing_system7_0/FCLK_RESET0_N] [get_bd_pins rst_processing_system7_0_96M/ext_reset_in]
  connect_bd_net -net rst_processing_system7_0_96M_interconnect_aresetn [get_bd_pins axi_mem_intercon/ARESETN] [get_bd_pins processing_system7_0_axi_periph/ARESETN] [get_bd_pins rst_processing_system7_0_96M/interconnect_aresetn]
  connect_bd_net -net rst_processing_system7_0_96M_peripheral_aresetn [get_bd_pins axi_mem_intercon/M00_ARESETN] [get_bd_pins axi_mem_intercon/S00_ARESETN] [get_bd_pins axi_mem_intercon/S01_ARESETN] [get_bd_pins axi_mem_intercon/S02_ARESETN] [get_bd_pins axi_mem_intercon/S03_ARESETN] [get_bd_pins axi_mem_intercon/S04_ARESETN] [get_bd_pins axi_mem_intercon/S05_ARESETN] [get_bd_pins axi_mem_intercon/S06_ARESETN] [get_bd_pins axi_mem_intercon/S07_ARESETN] [get_bd_pins axi_mem_intercon/S08_ARESETN] [get_bd_pins axi_mem_intercon/S09_ARESETN] [get_bd_pins axi_mem_intercon/S10_ARESETN] [get_bd_pins axi_mem_intercon/S11_ARESETN] [get_bd_pins axi_mem_intercon/S12_ARESETN] [get_bd_pins decode_main_0/aresetn] [get_bd_pins processing_system7_0_axi_periph/M00_ARESETN] [get_bd_pins processing_system7_0_axi_periph/S00_ARESETN] [get_bd_pins rst_processing_system7_0_96M/peripheral_aresetn]

  # Create address segments
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_NALU_BUF] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_PICORDER_NUM] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_MEMOCCUPIED] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_FRAME_NUM] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_TYPE] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_USE_FOR_CHAR] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_SLUMA] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_SCHROMA_0] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_SCHROMA_1] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_INFO_REFIDXL0] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_INFO_REFIDXL1] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_INFO_MVD_L0] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x20000000 -offset 0x0 [get_bd_addr_spaces decode_main_0/Data_M_AXI_PIC_INFO_MVD_L1] [get_bd_addr_segs processing_system7_0/S_AXI_HP0/HP0_DDR_LOWOCM] SEG_processing_system7_0_HP0_DDR_LOWOCM
  create_bd_addr_seg -range 0x10000 -offset 0x43C00000 [get_bd_addr_spaces processing_system7_0/Data] [get_bd_addr_segs decode_main_0/S_AXI_AXI4LITES/Reg] SEG_decode_main_0_Reg
  

  # Restore current instance
  current_bd_instance $oldCurInst

  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""
validate_bd_design

#Generate output design
generate_target all [get_files myproj/project_1.srcs/sources_1/bd/design_1/$design_name.bd]

#Create HDL wrapper
make_wrapper -files [get_files myproj/project_1.srcs/sources_1/bd/design_1/$design_name.bd] -top
add_files -norecurse myproj/project_1.srcs/sources_1/bd/design_1/hdl/${design_name}_wrapper.v
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

#Run synthesis, implementation, and write bitstream
reset_run synth_1
launch_runs impl_1 -to_step write_bitstream
wait_on_run synth_1
wait_on_run impl_1

