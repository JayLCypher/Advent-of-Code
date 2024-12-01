// See https://aka.ms/new-console-template for more information
namespace aocday7 {
	public enum CARD_TYPE {
		HIGH_CARD,
		ONE_PAIR,
		TWO_PAIR,
		THREE_OF_A_KIND,
		FULL_HOUSE,
		FOUR_OF_A_KIND,
		FIVE_OF_A_KIND,
		CARD_MAX
	};

	class CamelCard {
		public CARD_TYPE type;
		public int rank;
		public string card;
		public int bid;

		public CamelCard(CARD_TYPE aType, int aRank, string aCard, int aBid) {
			type = aType;
			rank = aRank;
			card = aCard;
			bid = aBid;
		}
	}

	class Program {
		static readonly bool do_example = true;
		static readonly string[] test_input = {
			"32T3K 765",
			"T55J5 684",
			"KK677 28",
			"KTJJT 220",
			"QQQJA 483",
		};
		static string[] input_from_file = { "", };
		static readonly char[] card_values = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2', }; // Ordered, highest to lowest

		static bool validate_card(string card) {
			foreach (char c in card) {
				bool is_valid = false;
				foreach (char v in card_values) {
					if (c == v) { is_valid = true; break; }
				}
				if (is_valid == false) { return is_valid; }
			}
			return true;
		}

		static CARD_TYPE parse_card(string card) {
			char[] sorted_card = card.ToCharArray();
			Array.Sort(sorted_card);
			Console.WriteLine(sorted_card);
			// AAAAA
			if (sorted_card[4] == sorted_card[3] && sorted_card[4] == sorted_card[2] && sorted_card[4] == sorted_card[1] && sorted_card[4] == sorted_card[0]) {
				return CARD_TYPE.FIVE_OF_A_KIND;
			}
			// AAAAX || XAAAA
			else if ((sorted_card[4] == sorted_card[3] && sorted_card[4] == sorted_card[2] && sorted_card[4] == sorted_card[1]) ||
					(sorted_card[3] == sorted_card[2] && sorted_card[3] == sorted_card[1] && sorted_card[3] == sorted_card[0])) {
				return CARD_TYPE.FOUR_OF_A_KIND;
			}
			// XAAAX
			else if (sorted_card[3] == sorted_card[2] && sorted_card[3] == sorted_card[1]) {
				return CARD_TYPE.THREE_OF_A_KIND;
			}
			// AAAXX || XXAAA
			else if ((sorted_card[4] == sorted_card[3] && sorted_card[4] == sorted_card[2]) ||
					(sorted_card[2] == sorted_card[1] && sorted_card[2] == sorted_card[0])) {
				// Need to determine if we're three-of-a-kind or full-house
				return (sorted_card[4] != sorted_card[2] && sorted_card[4] == sorted_card[3]) || (sorted_card[2] != sorted_card[0] && sorted_card[1] == sorted_card[0]) ? CARD_TYPE.FULL_HOUSE : CARD_TYPE.THREE_OF_A_KIND;
			}
			// AABBX || AAXBB || XAABB
			else if ((sorted_card[4] == sorted_card[3] && ((sorted_card[2] == sorted_card[1]) || sorted_card[1] == sorted_card[0])) ||
					(sorted_card[3] == sorted_card[2] && sorted_card[1] == sorted_card[0])) {
				return CARD_TYPE.TWO_PAIR;
			}
			// AAXXX || XAAXX || XXAAX || XXXAA
			else if ((sorted_card[4] == sorted_card[3]) || (sorted_card[3] == sorted_card[2]) || (sorted_card[2] == sorted_card[1]) || (sorted_card[1] == sorted_card[0])) {
				return CARD_TYPE.ONE_PAIR;
			}
			// ABCDE
			return CARD_TYPE.HIGH_CARD;
		}

		static void test_card_funcs() {
			// Test some parse_card cases
			System.Diagnostics.Debug.Assert(parse_card("AAAAA") == CARD_TYPE.FIVE_OF_A_KIND);
			System.Diagnostics.Debug.Assert(parse_card("AAAAK") == CARD_TYPE.FOUR_OF_A_KIND);
			System.Diagnostics.Debug.Assert(parse_card("KAAAA") == CARD_TYPE.FOUR_OF_A_KIND);
			System.Diagnostics.Debug.Assert(parse_card("AAKAA") == CARD_TYPE.FOUR_OF_A_KIND);
			System.Diagnostics.Debug.Assert(parse_card("AAKKA") == CARD_TYPE.FULL_HOUSE);
			System.Diagnostics.Debug.Assert(parse_card("AAKKK") == CARD_TYPE.FULL_HOUSE);
			System.Diagnostics.Debug.Assert(parse_card("KAKKA") == CARD_TYPE.FULL_HOUSE);
			System.Diagnostics.Debug.Assert(parse_card("KQAAA") == CARD_TYPE.THREE_OF_A_KIND);
			System.Diagnostics.Debug.Assert(parse_card("KQAAA") == CARD_TYPE.THREE_OF_A_KIND);
			System.Diagnostics.Debug.Assert(parse_card("KAAAQ") == CARD_TYPE.THREE_OF_A_KIND);
			System.Diagnostics.Debug.Assert(parse_card("KAQAA") == CARD_TYPE.THREE_OF_A_KIND);
			System.Diagnostics.Debug.Assert(parse_card("KAQAQ") == CARD_TYPE.TWO_PAIR);
			System.Diagnostics.Debug.Assert(parse_card("KAKAQ") == CARD_TYPE.TWO_PAIR);
			System.Diagnostics.Debug.Assert(parse_card("KA22J") == CARD_TYPE.ONE_PAIR);
			System.Diagnostics.Debug.Assert(parse_card("KAQAJ") == CARD_TYPE.ONE_PAIR);
		}

		static void Main(string[] args) {
			Console.WriteLine("Hello for Advent of Code, Day 7!");
			// Test card validation
			System.Diagnostics.Debug.Assert(validate_card("AAABA") == false);
			//if (do_example) { test_card_funcs(); }

			Console.WriteLine("Max values of Card_type: " + (int)CARD_TYPE.CARD_MAX);

			string[] input = do_example ? test_input : input_from_file;
			List<CamelCard>[] cards = new List<CamelCard>[(int)CARD_TYPE.CARD_MAX];
			for (int i = 0; i < (int)CARD_TYPE.CARD_MAX; ++i) {
				cards[i] = new List<CamelCard>();
			}
			foreach (string game in input) {
				Console.WriteLine(game);
				(string card, string bid) = game.Split(' ') switch { string[] a => (a[0], a[1]) };
				Console.WriteLine(card + bid);
				System.Diagnostics.Debug.Assert(validate_card(card) == true);
				CARD_TYPE type = parse_card(card);
				Console.WriteLine("Type: " + type);
				List<CamelCard> cur_list = cards[(int)type];
				bool inserted = false;
				for (int i = 0; i < card.Length; ++i) {
					foreach (CamelCard c in cur_list) {
						int idx = cur_list.IndexOf(c);
						if (Array.IndexOf(card_values, card[i]) < Array.IndexOf(card_values, c.card[i])) {
							cur_list.Insert(idx, new CamelCard(type, idx, card, int.Parse(bid)));
							inserted = true;
							break;
						}
					}
					if (inserted) { break; }
				}
				if (inserted == false) {
					cur_list.Add(new CamelCard(type, cur_list.Count, card, int.Parse(bid)));
				}
			}

			Console.WriteLine("CamelCards:");
			foreach (List<CamelCard> cs in cards) {
				foreach (CamelCard c in cs) {
					Console.WriteLine(c.type + " " + c.rank + " " + c.card + " " + c.bid);
				}
			}

		}
	}
}

