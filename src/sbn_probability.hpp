// Copyright 2019-2020 libsbn project contributors.
// libsbn is free software under the GPLv3; see LICENSE file for details.
//
// Perform training of an SBN based on a sample of trees.
//
// We assume that readers are familiar with how the sbn_parameters_ vector is laid out:
// first probabilities of rootsplits, then conditional probabilities of PCSPs.

#ifndef SRC_SBN_PROBABILITY_HPP_
#define SRC_SBN_PROBABILITY_HPP_

#include "eigen_sugar.hpp"
#include "sbn_maps.hpp"

namespace SBNProbability {

// The "SBN-SA" estimator described in the "Maximum Lower Bound Estimates" section of
// the 2018 NeurIPS paper.
void SimpleAverage(
    EigenVectorXdRef sbn_parameters,
    const UnrootedIndexerRepresentationCounter& indexer_representation_counter,
    size_t rootsplit_count, const BitsetSizePairMap& parent_to_range);

// The "SBN-EM" estimator described in the "Expectation Maximization" section of
// the 2018 NeurIPS paper. Returns the sequence of scores (defined in the paper)
// obtained by the EM iterations.
EigenVectorXd ExpectationMaximization(
    EigenVectorXdRef sbn_parameters,
    const UnrootedIndexerRepresentationCounter& indexer_representation_counter,
    size_t rootsplit_count, const BitsetSizePairMap& parent_to_range, double alpha,
    size_t max_iter, double score_epsilon);

// Calculate the probability of an indexer_representation of a topology.
double ProbabilityOf(EigenConstVectorXdRef,
                     const UnrootedIndexerRepresentation& indexer_representation);

// Calculate the probabilities of a collection of indexer_representations.
EigenVectorXd ProbabilityOf(
    EigenConstVectorXdRef sbn_parameters,
    const std::vector<UnrootedIndexerRepresentation>& indexer_representations);

// This function performs in-place normalization of vec given by range when its values
// are in log space.
void ProbabilityNormalizeRangeInLog(EigenVectorXdRef vec,
                                    std::pair<size_t, size_t> range);
// Perform in-place normalization of vec when its values are in log space.
// We assume that vec is laid out like sbn_parameters (see top).
void ProbabilityNormalizeParamsInLog(EigenVectorXdRef vec, size_t rootsplit_count,
                                     const BitsetSizePairMap& parent_to_range);
bool IsInSBNSupport(const SizeVector& rooted_representation,
                    size_t out_of_support_sentinel_value);

// Take the sum of the entries of vec in indices plus starting_value.
double SumOf(EigenConstVectorXdRef vec, const SizeVector& indices,
             double starting_value);

}  // namespace SBNProbability

#ifdef DOCTEST_LIBRARY_INCLUDED
   // Here we hardcode in "ground truth" values from
   // https://github.com/zcrabbit/sbn.
   // See https://github.com/phylovi/libsbn/pull/167 for details on how this code was
   // run.

EigenVectorXd ExpectedSAVector() {
  EigenVectorXd expected_SA(100);
  expected_SA << 0.1563122979972875, 0.1563122979972875, 0.1225902102462595,
      0.003813409758997299, 0.06405479308023015, 0.1225902102462595,
      0.006496265198833325, 0.07224488861161361, 0.07224488861161361,
      0.09211800063278303, 0.050235906509724905, 0.07224488861161361,
      0.1225902102462595, 0.004000036290989688, 0.000785970418989169,
      0.015740902738698836, 0.0007369945657169131, 0.092118000632783,
      0.050235906509724905, 0.15631229799728746, 0.004517219839302666,
      0.1225902102462595, 0.020070904829444434, 0.07224488861161361,
      0.00826101112145943, 0.1563122979972875, 0.050235906509724905, 0.092118000632783,
      6.6925344669117846e-06, 0.012000108872969078, 0.00107615168209648,
      0.00487602847011386, 0.00524108566424323, 0.1563122979972875,
      0.006470871758283066, 0.050235906509724905, 0.0034098101830328945,
      0.1563122979972875, 0.07224488861161361, 0.0036073007280301274,
      0.009488612393535554, 0.005657493542553093, 0.007936324421697116,
      0.1225902102462595, 0.1563122979972875, 0.0064788184404525545, 0.1563122979972875,
      0.006493612301549656, 0.1225902102462595, 0.1225902102462595, 0.06405479308023015,
      0.06405479308023015, 0.092118000632783, 0.006224174813063337,
      0.006496265198833326, 0.15631229799728746, 0.002156957252761021,
      0.008283255738394914, 0.012789795178479234, 0.1225902102462595,
      0.0057598153715508514, 0.1225902102462595, 0.1563122979972875,
      0.12259021024625949, 0.15631229799728746, 0.004505082963907347,
      0.15631229799728746, 0.06405479308023015, 0.050235906509724905,
      0.00016398394968572145, 0.1225902102462595, 0.15631229799728752,
      0.050235906509724905, 0.1563122979972875, 0.003933969974285363,
      0.09211800063278297, 0.07224488861161361, 0.1563122979972875, 0.12259021024625949,
      0.012098733104319886, 0.00028556179453190954, 0.005744340855421819,
      0.00299072194405209, 0.0031839409290006357, 0.092118000632783, 0.1225902102462595,
      0.0015789121009827038, 0.1563122979972875, 0.1225902102462595,
      0.009488612393535554, 0.008035213534600344, 0.008283255738394914,
      0.1225902102462595, 0.1563122979972875, 0.012789795178479234, 0.00826101112145943,
      0.15631229799728746, 0.1563122979972875, 0.015740902738698836,
      0.0014116239862321806;
  return expected_SA;
}

// Expected EM vectors with alpha = 0.
std::tuple<EigenVectorXd, EigenVectorXd> ExpectedEMVectorsAlpha0() {
  // 1 iteration of EM with alpha = 0.
  EigenVectorXd expected_EM_0_1(100);
  expected_EM_0_1 << 0.15636219370379975, 0.15636219370379975, 0.12263720847530954,
      0.0038161261257420274, 0.0641198257552132, 0.12263720847530954,
      0.006486659269203554, 0.07229291766902365, 0.07229291766902365,
      0.09217334703350938, 0.05029011931468532, 0.07229291766902365,
      0.12263720847530954, 0.004003916595779366, 0.0007856587472007348,
      0.01573322403407416, 0.0007374660239687015, 0.09217334703350938,
      0.05029011931468532, 0.15636219370379975, 0.004512401354352734,
      0.12263720847530954, 0.02005981904435064, 0.07229291766902365,
      0.008265715290818319, 0.15636219370379975, 0.05029011931468532,
      0.09217334703350938, 6.696764561669613e-06, 0.0120117421642559,
      0.0010771644269441463, 0.004896166585246872, 0.005249064166033721,
      0.15636219370379975, 0.006473675486085066, 0.05029011931468532,
      0.0032873955583662736, 0.15636219370379975, 0.07229291766902365,
      0.0036100017361450775, 0.009434696399838294, 0.0056590388012875215,
      0.007977630170932788, 0.12263720847530954, 0.15636219370379975,
      0.006482919565829875, 0.15636219370379975, 0.006552518113293269,
      0.12263720847530954, 0.12263720847530954, 0.0641198257552132, 0.0641198257552132,
      0.09217334703350938, 0.0062569566301722964, 0.006486659269203554,
      0.15636219370379975, 0.002157895979240378, 0.008270476015894701,
      0.012800407342249945, 0.12263720847530954, 0.0057533024269482485,
      0.12263720847530954, 0.15636219370379975, 0.12263720847530954,
      0.15636219370379975, 0.004509802427487057, 0.15636219370379975,
      0.0641198257552132, 0.05029011931468532, 0.00016356223078203, 0.12263720847530954,
      0.15636219370379975, 0.05029011931468532, 0.15636219370379975,
      0.0039366703105347105, 0.09217334703350938, 0.07229291766902365,
      0.15636219370379975, 0.12263720847530954, 0.01202920464244238,
      0.00028206667942050513, 0.005749993943460951, 0.0029930413584166584,
      0.00329320520591652, 0.09217334703350938, 0.12263720847530954,
      0.0015794165564839767, 0.15636219370379975, 0.12263720847530954,
      0.009434696399838294, 0.0080145373179354, 0.008270476015894701,
      0.12263720847530954, 0.15636219370379975, 0.012800407342249945,
      0.008265715290818319, 0.15636219370379975, 0.15636219370379975,
      0.01573322403407416, 0.0014108855861473272;

  // 23 iterations of EM with alpha = 0.
  EigenVectorXd expected_EM_0_23(100);
  expected_EM_0_23 << 0.17652149361215352, 0.17652149361215352, 0.13955673648946823,
      0.0064491608851600735, 0.05848390318274005, 0.13955673648946823,
      0.015825262650921094, 0.056647494412346275, 0.056647494412346275,
      0.07263326598713499, 0.046048205076811774, 0.056647494412346275,
      0.13955673648946823, 0.004489402988562556, 0.0008454696589522312,
      0.01696511452269485, 0.0007597630896160637, 0.07263326598713499,
      0.046048205076811774, 0.17652149361215352, 0.007245923330084535,
      0.13955673648946823, 0.021613944434184986, 0.056647494412346275,
      0.014714698661351094, 0.17652149361215352, 0.046048205076811774,
      0.07263326598713499, 9.203216973858281e-06, 0.012351059173222767,
      0.0009871200936099765, 0.004930024591016917, 0.00491121394019874,
      0.17652149361215352, 0.003698082517142961, 0.046048205076811774,
      0.005102502844331727, 0.17652149361215352, 0.056647494412346275,
      0.002888359861489329, 0.010768901737247942, 0.004400293712986419,
      0.009082480764670433, 0.13955673648946823, 0.17652149361215352,
      0.011355455748479546, 0.1765214936121535, 0.004904756855047689,
      0.13955673648946823, 0.13955673648946823, 0.05848390318274005,
      0.05848390318274005, 0.07263326598713499, 0.0071809191341133454,
      0.015825262650921094, 0.17652149361215352, 0.002748656816901356,
      0.020534769315758854, 0.015225481783759537, 0.13955673648946823,
      0.009401422446411185, 0.13955673648946823, 0.17652149361215352,
      0.13955673648946823, 0.17652149361215352, 0.005835611123722098,
      0.17652149361215352, 0.05848390318274005, 0.046048205076811774,
      0.00021701159553434837, 0.13955673648946823, 0.17652149361215352,
      0.046048205076811774, 0.17652149361215352, 0.003632704913367913,
      0.07263326598713499, 0.056647494412346275, 0.17652149361215352,
      0.13955673648946823, 0.013620606755282007, 0.0013952624329697414,
      0.007542542618796534, 0.004980505041486884, 0.0049091058169283925,
      0.07263326598713499, 0.13955673648946823, 0.0018467480153313562,
      0.17652149361215352, 0.13955673648946823, 0.010768901737247942,
      0.00801061474692208, 0.020534769315758854, 0.13955673648946823,
      0.17652149361215352, 0.015225481783759537, 0.014714698661351094,
      0.1765214936121535, 0.17652149361215352, 0.01696511452269485,
      0.0010195073633053277;

  return {expected_EM_0_1, expected_EM_0_23};
}

// Expected EM vector with alpha = 0.5
EigenVectorXd ExpectedEMVectorAlpha05() {
  // 100 iterations of EM with alpha = 0.5, from Andy's Rev implementation.
  EigenVectorXd expected_EM_05_100(100);
  expected_EM_05_100 << 0.156334, 0.156334, 0.1226123, 0.003816548, 0.06410458,
      0.1226123, 0.006490481, 0.07228169, 0.07228169, 0.09216116, 0.05027706,
      0.07228169, 0.1226123, 0.00400394, 0.0007856247, 0.01573095, 0.0007376477,
      0.09216116, 0.05027706, 0.156334, 0.004514539, 0.1226123, 0.02005742, 0.07228169,
      0.008265798, 0.156334, 0.05027706, 0.09216116, 6.695972e-06, 0.01201178,
      0.001077308, 0.004899056, 0.005247481, 0.156334, 0.006473987, 0.05027706,
      0.003445344, 0.156334, 0.07228169, 0.0036098, 0.009448465, 0.005659673,
      0.007976663, 0.1226123, 0.156334, 0.006482844, 0.156334, 0.006417704, 0.1226123,
      0.1226123, 0.06410458, 0.06410458, 0.09216116, 0.006256103, 0.006490481, 0.156334,
      0.002157991, 0.008275552, 0.01279726, 0.1226123, 0.005756163, 0.1226123, 0.156334,
      0.1226123, 0.156334, 0.004509251, 0.156334, 0.06410458, 0.05027706, 0.0001636864,
      0.1226123, 0.156334, 0.05027706, 0.156334, 0.003936684, 0.09216116, 0.07228169,
      0.156334, 0.1226123, 0.01204707, 0.0002831188, 0.005749409, 0.002993311,
      0.003384393, 0.09216116, 0.1226123, 0.001579132, 0.156334, 0.1226123, 0.009448465,
      0.008020635, 0.008275552, 0.1226123, 0.156334, 0.01279726, 0.008265798, 0.156334,
      0.156334, 0.01573095, 0.001410898;
  return expected_EM_05_100;
}

#endif  // DOCTEST_LIBRARY_INCLUDED
#endif  // SRC_SBN_PROBABILITY_HPP_
