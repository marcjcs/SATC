import ShopListItem from '../Components/ShopListItem';

function ShopList({ items, addToCart }) {
  return items.map((item) => (
    <ShopListItem
      title={item.title}
      description={item.description}
      price={item.price}
      image={item.image}
      addToCart={() => addToCart(item)}
      key={item.id}
    />
  ));
}

export default ShopList;
