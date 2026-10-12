#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
namespace sigilbound {
struct MoveSlot {
    std::uint16_t id=0;
    std::uint8_t uses=0, maximum=0;
};
struct BeastRecord {
    std::uint32_t id=0;
    std::uint16_t species=0, hp=0, maximum_hp=0;
    std::array<MoveSlot,4> moves{};
};
inline bool valid_beast(const BeastRecord& b) {
    if (!b.id || !b.species || !b.maximum_hp || b.hp>b.maximum_hp) return false;
    for (std::size_t i=0;i<b.moves.size();++i) {
        const auto& m=b.moves[i];
        if ((!m.id && (m.uses || m.maximum)) || (m.id && (!m.maximum || m.uses>m.maximum))) return false;
        for (std::size_t j=0;j<i;++j)
            if (m.id && m.id==b.moves[j].id) return false;
    }
    return true;
}
enum class SwitchOutcome { rejected, voluntary_turn, forced_replacement };
class Party {
    std::array<BeastRecord,5> beasts_{};
    std::size_t size_=0, active_=0;
public:
    std::size_t size() const { return size_; }
    std::size_t active_index() const { return active_; }
    const BeastRecord* beast(std::size_t i) const { return i<size_ ? &beasts_[i] : nullptr; }
    bool add(const BeastRecord& b) {
        if (size_==beasts_.size() || !valid_beast(b) || (!size_ && !b.hp)) return false;
        for (std::size_t i=0;i<size_;++i) if (beasts_[i].id==b.id) return false;
        beasts_[size_++]=b; return true;
    }
    bool defeated() const {
        for (std::size_t i=0;i<size_;++i) if (beasts_[i].hp) return false;
        return true;
    }
    SwitchOutcome switch_to(std::size_t i) {
        if (i>=size_ || i==active_ || !beasts_[i].hp) return SwitchOutcome::rejected;
        const auto outcome=beasts_[active_].hp ? SwitchOutcome::voluntary_turn : SwitchOutcome::forced_replacement;
        active_=i; return outcome;
    }
    bool damage_active(std::uint16_t amount) {
        if (!size_ || !beasts_[active_].hp) return false;
        auto& hp=beasts_[active_].hp;
        hp=amount>=hp ? 0 : static_cast<std::uint16_t>(hp-amount);
        return true;
    }
    bool consume_move(std::size_t slot) {
        if (!size_ || !beasts_[active_].hp || slot>=4) return false;
        auto& m=beasts_[active_].moves[slot];
        if (!m.id || !m.uses) return false;
        --m.uses; return true;
    }
};
}
