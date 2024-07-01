void DrawKyotoMulti(int run_number) {
	auto file =
		new TFile(Form("/home/s063/exp/exp2406_s063/anaroot/users/kin/scratch/ana/analyzed/data%d.root", run_number));

	if (!file->IsOpen()) {
		cout << "== File is bad!" << endl;
		return;
	}

	auto tree = (TTree *)file->Get("samurai");
	auto c = new TCanvas("kyoto_mult", "", 1200, 800);
	tree->Draw("kyoto_multi");
	c->SaveAs(Form("kyoto_mult_%d.png", run_number));
}
