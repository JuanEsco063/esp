/* Copyright 2017 Columbia University, SLD Group */

#ifndef __LLC_HPP__
#define __LLC_HPP__

#include "cache_utils.hpp"
#include "llc_directives.hpp"
// #include "llc_tags.hpp"
// #include "llc_states.hpp"
// #include "llc_hprots.hpp"
// #include "llc_lines.hpp"
// #include "llc_sharers.hpp"
// #include "llc_owners.hpp"
// #include "llc_evict_ways.hpp"

class llc : public sc_module
{

public:

    // Clock signal
    sc_in<bool> clk;

    // Reset signal
    sc_in<bool> rst;

    // Debug signals
    sc_out< sc_bv<LLC_ASSERT_WIDTH> >   asserts;
    sc_out< sc_bv<LLC_BOOKMARK_WIDTH> > bookmark;
    sc_out<uint32_t>                    custom_dbg;

#ifdef LLC_DEBUG

    sc_out<bool> tag_hit_out;
    sc_out<llc_way_t> hit_way_out;
    sc_out<bool> empty_way_found_out;
    sc_out<llc_way_t> empty_way_out;
    sc_out<bool> evict_out;
    sc_out<llc_way_t> way_out;
    sc_out<llc_addr_t> llc_addr_out;

    sc_out<bool> req_stall_out;
    sc_out<bool> req_in_stalled_valid_out;
    sc_out<llc_req_in_t> req_in_stalled_out;

    sc_out<bool> is_rsp_to_get_out;
    sc_out<bool> is_req_to_get_out;

    sc_out<tag_t> tag_buf_out[LLC_WAYS];
    sc_out<llc_state_t> state_buf_out[LLC_WAYS];
    sc_out<sharers_t> sharers_buf_out[LLC_WAYS];
    sc_out<owner_t> owner_buf_out[LLC_WAYS];

#endif

    // Input ports
    nb_get_initiator<llc_req_in_t>	llc_req_in;
    nb_get_initiator<llc_rsp_in_t>	llc_rsp_in;
    get_initiator<llc_mem_rsp_t>	llc_mem_rsp;
    nb_get_initiator<bool>              llc_rst_tb;

    // Output ports
    put_initiator<llc_rsp_out_t>	llc_rsp_out;
    put_initiator<llc_fwd_out_t>	llc_fwd_out;
    put_initiator<llc_mem_req_t>        llc_mem_req;
    put_initiator<bool>                 llc_rst_tb_done;

    // Local memory
    tag_t tags[LLC_LINES];
    llc_state_t states[LLC_LINES];
    hprot_t hprots[LLC_LINES];
    line_t lines[LLC_LINES];
    sharers_t sharers[LLC_LINES];
    owner_t owners[LLC_LINES];
    // llc_way_t evict_ways[SETS];

    // Local registers
    tag_t	 tag_buf[LLC_WAYS];
    llc_state_t	 state_buf[LLC_WAYS];
    hprot_t	 hprot_buf[LLC_WAYS];
    line_t	 line_buf[LLC_WAYS];
    sharers_t	 sharers_buf[LLC_WAYS];
    owner_t      owner_buf[LLC_WAYS];
    // llc_way_t	 evict_way_buf;

    // Constructor
    SC_CTOR(llc)
	    : clk("clk")
	    , rst("rst")
	    , asserts("asserts")
	    , bookmark("bookmark")
	    , custom_dbg("custom_dbg")
	    , tag_hit_out("tag_hit_out")
	    , hit_way_out("hit_way_out")
	    , empty_way_found_out("empty_way_found_out")
	    , empty_way_out("empty_way_out")
	    , evict_out("evict_out")
	    , way_out("way_out")
	    , llc_addr_out("llc_addr_out")
	    , llc_req_in("llc_req_in")
	    , llc_rsp_in("llc_rsp_in")
	    , llc_mem_rsp("llc_mem_rsp")
	    , llc_rst_tb("llc_rst_tb")
	    , llc_rsp_out("llc_rsp_out")
	    , llc_fwd_out("llc_fwd_out")
	    , llc_mem_req("llc_mem_req")
	    , llc_rst_tb_done("llc_rst_tb_done")
    {
        // Cache controller process
	SC_CTHREAD(ctrl, clk.pos());
	reset_signal_is(rst, false);
	// set_stack_size(0x400000);

	// Assign clock and reset to put_get ports
	llc_req_in.clk_rst (clk, rst);
	llc_rsp_in.clk_rst (clk, rst);
	llc_mem_rsp.clk_rst (clk, rst);
	llc_rst_tb.clk_rst(clk, rst);
	llc_rsp_out.clk_rst (clk, rst);
	llc_fwd_out.clk_rst(clk, rst);
	llc_mem_req.clk_rst(clk, rst);
	llc_rst_tb_done.clk_rst(clk, rst);

	// Flatten arrays
	LLC_FLATTEN_REGS;

	// Map arrays to memory
#if (N_CPU == 2)
	HLS_MAP_TO_MEMORY(tags, "llc_tags_2cpus");
	HLS_MAP_TO_MEMORY(states, "llc_states_2cpus");
	HLS_MAP_TO_MEMORY(hprots, "llc_hprots_2cpus");
	HLS_MAP_TO_MEMORY(lines, "llc_lines_2cpus");
	HLS_MAP_TO_MEMORY(sharers, "llc_sharers_2cpus");
	HLS_MAP_TO_MEMORY(owners, "llc_owners_2cpus");
#else
	HLS_MAP_TO_MEMORY(tags, "llc_tags_4cpus");
	HLS_MAP_TO_MEMORY(states, "llc_states_4cpus");
	HLS_MAP_TO_MEMORY(hprots, "llc_hprots_4cpus");
	HLS_MAP_TO_MEMORY(lines, "llc_lines_4cpus");
	HLS_MAP_TO_MEMORY(sharers, "llc_sharers_4cpus");
	HLS_MAP_TO_MEMORY(owners, "llc_owners_4cpus");
#endif
	// HLS_MAP_TO_MEMORY(evict_ways, "llc_evict_ways");
    }

    // Processes
    void ctrl(); // cache controller

    // Functions
    inline void reset_io();
    inline void reset_states();
    void read_set(llc_addr_t base, llc_way_t way_base);
    void lookup(tag_t tag, set_t set, llc_way_t &way, bool &evict, llc_addr_t &llc_addr);
    void send_mem_req(bool hwrite, addr_t line_addr, hprot_t hprot, line_t line);
    void get_mem_rsp(line_t &line);
    void get_req_in(llc_req_in_t &req_in);
    void get_rsp_in(llc_rsp_in_t &rsp_in);
    void send_rsp_out(coh_msg_t coh_msg, addr_t addr, line_t line, cache_id_t req_id,
		      cache_id_t dest_id, invack_cnt_t invack_cnt);
    void send_fwd_out(coh_msg_t coh_msg, addr_t addr, cache_id_t req_id, cache_id_t dest_id);

private:
    // debug
    sc_bv<LLC_ASSERT_WIDTH>   asserts_tmp;
    sc_bv<LLC_BOOKMARK_WIDTH> bookmark_tmp;
    uint64_t custom_dbg_tmp;
    bool req_stall;
    bool req_in_stalled_valid;
    llc_req_in_t req_in_stalled;
};

#endif /* __LLC_HPP__ */
