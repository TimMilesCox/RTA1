pushd	../temp.localhost
masmx	bpf_gate bpf_gate -wk
bpf_slab bpf_gate.txo filter
physal	localhost
popd

