pushd	../temp.ethernet
masmx	bpf_gate bpf_gate -wk
bpf_slab bpf_gate.txo filter
physal	ethernet
popd

